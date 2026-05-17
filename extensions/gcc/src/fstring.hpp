#ifndef FSTRING_HPP
#define FSTRING_HPP

#include "config.h"
#include "system.h"
#include <string>
#include <vector>
#include <map>
#include <optional>
#include "coretypes.h"
#include "c-family/c-common.h"
#include "gcc-plugin.h"
#include "plugin-version.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "diagnostic-core.h"
#include "toplev.h"
#include "flags.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "context.h"
#include "cgraph.h"
#include "langhooks.h"
#include "real.h"
#include <algorithm>
#include "gimple.h"
#include "gimple-iterator.h"
#include "gimple-expr.h"
#include "tree-dfa.h"
#include "gimplify.h"

#ifdef __cplusplus
extern "C" {
#endif
#undef __counted_by
#include "yio/private/yio_buf.h"
#ifdef __cplusplus
}
#endif

extern int plugin_is_GPL_compatible;

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

// Global flags
extern bool warn_fstring_format;
extern bool error_fstring_format;
extern bool fstring_optimizations;

#endif // FSTRING_HPP
