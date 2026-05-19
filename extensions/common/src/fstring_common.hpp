#ifndef FSTRING_COMMON_HPP
#define FSTRING_COMMON_HPP

#include <string>
#include <vector>
#include <optional>
#include <functional>
#include <cstring>
#include <cctype>

struct FStringToken {
  bool is_text = false;
  std::string text;      // Literal text or full placeholder string
  std::string expr;      // Variable/expression name
  std::string spec;      // Format specifier excluding colon
  std::optional<std::string> width;     // Extracted nested expression for width
  std::optional<std::string> precision; // Extracted nested expression for precision

  // Python-style format specifier components
  char fill = '\0';
  char align = '\0';
  char sign = '\0';
  bool z = false;
  bool hash = false;
  bool zero = false;
  char grouping_option = '\0';
  bool locale = false;
  char type = '\0';
  bool parsed_to_end = false;
};

template <typename LocType>
class FStringParserBase {
public:
  using DiagnosticReporter = std::function<void(LocType, const std::string&)>;

protected:
  DiagnosticReporter Reporter;

public:
  FStringParserBase(DiagnosticReporter Reporter) : Reporter(Reporter) {}

  void parse_specifier(FStringToken &tok, LocType loc) {
    if (tok.spec.empty()) {
      tok.parsed_to_end = true;
      return;
    }
    const char *p = tok.spec.c_str();

    if (*p && *(p + 1) && strchr("<>=^", *(p + 1))) {
      tok.fill = *p;
      tok.align = *(p + 1);
      if (*p == '{' || *p == '}') {
        Reporter(loc, "brace is not allowed as a fill character");
      }
      p += 2;
    } else if (*p && strchr("<>=^", *p)) {
      tok.align = *p;
      p++;
    }

    while (*p && strchr("+- z#0", *p)) {
      if (*p == '+' || *p == '-' || *p == ' ') tok.sign = *p;
      else if (*p == 'z') tok.z = true;
      else if (*p == '#') tok.hash = true;
      else if (*p == '0') tok.zero = true;
      p++;
    }

    while (*p && isdigit(*p)) p++;

    if (*p && strchr("_,", *p)) {
      tok.grouping_option = *p;
      p++;
    }

    if (*p == '.') {
      p++;
      while (*p && isdigit(*p)) p++;
    }

    if (*p == 'L') {
      tok.locale = true;
      p++;
    }

    if (*p && isalpha(*p)) {
      tok.type = *p;
      p++;
    }

    tok.parsed_to_end = (*p == '\0');
  }

  std::vector<FStringToken> parse(const char *str, LocType loc) {
    std::vector<FStringToken> tokens;
    const char *p = str;
    bool auto_mode = false;
    bool manual_mode = false;

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
            if (*p == '{') break;
            tok.expr += *p;
            tok.text += *p++;
          }

          if (tok.expr.empty()) {
            if (manual_mode) Reporter(loc, "cannot switch from manual field specification to automatic field numbering");
            auto_mode = true;
          } else {
            char *endptr;
            strtol(tok.expr.c_str(), &endptr, 10);
            if (*endptr == '\0') {
              if (auto_mode) Reporter(loc, "cannot switch from automatic field numbering to manual field specification");
              manual_mode = true;
            }
          }

          if (*p == '!') {
            Reporter(loc, "conversion specifier '!' is not supported");
            while (*p && *p != '}' && *p != ':') tok.text += *p++;
          }
          if (*p == ':') {
            tok.text += *p++;
            const char *spec_start = p;
            while (*p && *p != '}') {
              if (*p == '{') {
                bool is_precision = (p > spec_start && *(p - 1) == '.');
                tok.text += *p++;
                if (*p == '{') {
                  Reporter(loc, "nested '{{' inside format specifier is not allowed");
                  tok.text += *p++;
                  continue;
                }
                std::string inner;
                while (*p && *p != '}' && *p != ':') {
                  if (*p == '{') Reporter(loc, "nested replacement fields are not allowed");
                  inner += *p;
                  tok.text += *p++;
                }
                if (*p == ':') {
                  Reporter(loc, "nested format specifier not allowed in dynamic field");
                  while (*p && *p != '}') tok.text += *p++;
                }
                if (*p == '}') {
                  if (inner.empty()) {
                    if (manual_mode) Reporter(loc, "cannot switch from manual field specification to automatic field numbering");
                    auto_mode = true;
                  } else {
                    char *endptr;
                    strtol(inner.c_str(), &endptr, 10);
                    if (*endptr == '\0') {
                      if (auto_mode) Reporter(loc, "cannot switch from automatic field numbering to manual field specification");
                      manual_mode = true;
                    }
                  }
                  if (is_precision) tok.precision = inner;
                  else tok.width = inner;
                  tok.text += *p++;
                }
              } else {
                tok.spec += *p;
                tok.text += *p++;
              }
            }
            parse_specifier(tok, loc);
          }
          if (*p == '}') {
            tok.text += *p++;
            tokens.push_back(tok);
          } else {
            Reporter(loc, "unclosed '{' in f-string");
          }
        }
      } else if (*p == '}') {
        p++;
        if (*p == '}') {
          tokens.push_back({true, "}"});
          p++;
        } else {
          Reporter(loc, "unmatched '}' in f-string");
        }
      } else {
        std::string text;
        while (*p && *p != '{' && *p != '}') text += *p++;
        tokens.push_back({true, text});
      }
    }
    return tokens;
  }
};

#endif // FSTRING_COMMON_HPP
