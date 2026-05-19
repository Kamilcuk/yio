#ifndef FSTRING_PARSER_HPP
#define FSTRING_PARSER_HPP

#include "fstring_common.hpp"
#include "fstring_utils.hpp"

class FStringParser : public FStringParserBase<location_t> {
public:
  FStringParser(location_t loc) 
    : FStringParserBase<location_t>([this](location_t l, const std::string& msg) {
        report_fstring_problem(l, "%s", msg.c_str());
      }) {}

  // Shadow parse to use the instance loc if needed, or just use the base parse.
  // The base parse takes LocType as argument to parse methods.
};

static inline std::vector<FStringToken> parse_fstring(const char *str, location_t loc) {
  FStringParser parser(loc);
  return parser.parse(str, loc);
}

// validate_type_spec remains here as it's GCC specific (uses tree)
static inline void validate_type_spec(tree type, const FStringToken &tok, location_t loc) {
  if (tok.spec.empty()) { return; }

  if (type != NULL_TREE && is_optimizable_type(type) && !tok.parsed_to_end) {
    report_fstring_problem(loc, "invalid format specification");
  }

  char type_char = tok.type;
  if (type == NULL_TREE) { return; }

  if (INTEGRAL_TYPE_P(type)) {
    const char *integral_specs = "duoxbDUOXBc";
    if (type_char != '\0' && !strchr(integral_specs, type_char)) {
      char tc[2] = {type_char, '\0'};
      report_fstring_problem(loc, "invalid format specifier %s for %qT. use one of %qs instead", tc, type, integral_specs);
    }
  } else if (SCALAR_FLOAT_TYPE_P(type)) {
    const char *float_specs = "fegaFEGA";
    if (type_char != '\0' && !strchr(float_specs, type_char)) {
      char tc[2] = {type_char, '\0'};
      report_fstring_problem(loc, "invalid format specifier %s for %qT. use one of %qs instead", tc, type, float_specs);
    }
  } else if (POINTER_TYPE_P(type)) {
    tree target = TREE_TYPE(type);
    if (target && TYPE_MAIN_VARIANT(target) == char_type_node) {
      if (type_char != '\0' && type_char != 's') {
        char tc[2] = {type_char, '\0'};
        report_fstring_problem(loc, "invalid format specifier %s for %qT. use %qs instead", tc, type, "s");
      }
    } else {
      if (type_char != '\0' && type_char != 'p') {
        char tc[2] = {type_char, '\0'};
        report_fstring_problem(loc, "invalid format specifier %s for %qT. use %qs instead", tc, type, "p");
      }
    }
  }
}

#endif // FSTRING_PARSER_HPP
