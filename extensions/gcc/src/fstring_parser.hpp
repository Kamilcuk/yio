#ifndef FSTRING_PARSER_HPP
#define FSTRING_PARSER_HPP

#include "fstring_utils.hpp"

static inline void parse_specifier(FStringToken &tok, location_t loc) {
  if (tok.spec.empty()) {
    tok.parsed_to_end = true;
    return;
  }
  const char *p = tok.spec.c_str();

  if (*p && *(p + 1) && strchr("<>=^", *(p + 1))) {
    tok.fill = *p;
    tok.align = *(p + 1);
    if (*p == '{' || *p == '}') {
      report_fstring_problem(loc, "brace %qc is not allowed as a fill character", *p);
    }
    p += 2;
  } else if (*p && strchr("<>=^", *p)) {
    tok.align = *p;
    p++;
  }

  while (*p && strchr("+- z#0", *p)) {
    if (*p == '+' || *p == '-' || *p == ' ') {
      tok.sign = *p;
    } else if (*p == 'z') {
      tok.z = true;
    } else if (*p == '#') {
      tok.hash = true;
    } else if (*p == '0') {
      tok.zero = true;
    }
    p++;
  }

  while (*p && ISDIGIT(*p)) { p++; }

  if (*p && strchr("_,", *p)) {
    tok.grouping_option = *p;
    p++;
  }

  if (*p == '.') {
    p++;
    while (*p && ISDIGIT(*p)) { p++; }
  }

  if (*p == 'L') {
    tok.locale = true;
    p++;
  }

  if (*p && ISALPHA(*p)) {
    tok.type = *p;
    p++;
  }

  tok.parsed_to_end = (*p == '\0');
}

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

static inline std::vector<FStringToken> parse_fstring(const char *str, location_t loc) {
  std::vector<FStringToken> tokens;
  const char *p = str;
  enum IndexingMode {
    INDEX_MODE_UNSET,
    INDEX_MODE_AUTO,
    INDEX_MODE_MANUAL
  } mode = INDEX_MODE_UNSET;

  while (*p) {
    if (*p == '{') {
      p++;
      if (*p == '{') {
        tokens.push_back({true, "{"});
        p++;
      } else {
        FStringToken tok;
        tok.is_text = false;
        tok.text = "{";
        while (*p && *p != '}' && *p != ':' && *p != '!') {
          if (*p == '{') { break; }
          tok.expr += *p;
          tok.text += *p++;
        }

        if (tok.expr.empty()) {
          if (mode == INDEX_MODE_MANUAL) {
            report_fstring_problem(loc, "cannot switch from manual field specification to automatic field numbering");
          }
          mode = INDEX_MODE_AUTO;
        } else {
          char *endptr;
          strtol(tok.expr.c_str(), &endptr, 10);
          if (*endptr == '\0') {
            if (mode == INDEX_MODE_AUTO) {
              report_fstring_problem(loc, "cannot switch from automatic field numbering to manual field specification");
            }
            mode = INDEX_MODE_MANUAL;
          }
        }

        if (*p == '!') {
          report_fstring_problem(loc, "conversion specifier %<!%> is not supported");
          while (*p && *p != '}' && *p != ':') { tok.text += *p++; }
        }
        if (*p == ':') {
          tok.text += *p++;
          const char *spec_start = p;
          while (*p && *p != '}') {
            if (*p == '{') {
              bool is_precision = (p > spec_start && *(p - 1) == '.');
              tok.text += *p++;
              if (*p == '{') {
                report_fstring_problem(loc, "nested %<{{%> inside format specifier is not allowed");
                tok.text += *p++;
                continue;
              }
              std::string inner;
              while (*p && *p != '}' && *p != ':') {
                if (*p == '{') {
                  report_fstring_problem(loc, "nested replacement fields are not allowed");
                }
                inner += *p;
                tok.text += *p++;
              }
              if (*p == ':') {
                report_fstring_problem(loc, "nested format specifier not allowed in dynamic field. remove the inner %<:%>");
                while (*p && *p != '}') { tok.text += *p++; }
              }
              if (*p == '}') {
                if (inner.empty()) {
                  if (mode == INDEX_MODE_MANUAL) {
                    report_fstring_problem(loc, "cannot switch from manual field specification to automatic field numbering");
                  }
                  mode = INDEX_MODE_AUTO;
                } else {
                  char *endptr;
                  strtol(inner.c_str(), &endptr, 10);
                  if (*endptr == '\0') {
                    if (mode == INDEX_MODE_AUTO) {
                      report_fstring_problem(loc, "cannot switch from automatic field numbering to manual field specification");
                    }
                    mode = INDEX_MODE_MANUAL;
                  }
                }

                if (is_precision) {
                  tok.precision = inner;
                } else {
                  tok.width = inner;
                }
                tok.text += *p++;
              }
            } else {
              tok.spec += *p;
              tok.text += *p++;
            }
          }
          parse_specifier(tok, loc);
          validate_type_spec(NULL_TREE, tok, loc);
        }
        if (*p == '}') {
          tok.text += *p++;
          tokens.push_back(tok);
        } else {
          report_fstring_problem(loc, "unclosed replacement field. add a matching %<}%> or escape as %<{{%>");
        }
      }
    } else if (*p == '}') {
      p++;
      if (*p == '}') {
        tokens.push_back({true, "}"});
        p++;
      } else {
        report_fstring_problem(loc, "unmatched %<}%> in f-string. remove it or escape as %<}}%>");
      }
    } else {
      std::string text;
      while (*p && *p != '{' && *p != '}') { text += *p++; }
      tokens.push_back({true, text});
    }
  }
  return tokens;
}

#endif // FSTRING_PARSER_HPP
