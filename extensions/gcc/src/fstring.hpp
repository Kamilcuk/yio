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

// Global flags
extern bool warn_fstring_format;
extern bool error_fstring_format;
extern bool fstring_optimizations;

#endif // FSTRING_HPP
