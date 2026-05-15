#include "config.h"
#include "system.h"
#include <string>
#include <vector>
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

int plugin_is_GPL_compatible;

static int get_fmt_index(tree fn) {
    tree attr = lookup_attribute("yio_format", DECL_ATTRIBUTES(fn));
    if (!attr) return -1;
    tree args = TREE_VALUE(attr);
    if (!args || TREE_CODE(args) != TREE_LIST) return 0;
    tree val = TREE_VALUE(args);
    if (val && TREE_CODE(val) == INTEGER_CST) return (int)TREE_INT_CST_LOW(val) - 1;
    return 0;
}

static bool is_integer_type(tree type) {
    if (!type) return false;
    return TREE_CODE(type) == INTEGER_TYPE || TREE_CODE(type) == BOOLEAN_TYPE || TREE_CODE(type) == ENUMERAL_TYPE;
}

static void validate_yio_call(location_t loc, tree call, int fmt_idx) {
    if (call_expr_nargs(call) <= (unsigned int)fmt_idx) return;
    tree fmt_arg = CALL_EXPR_ARG(call, fmt_idx);
    while (TREE_CODE(fmt_arg) == NOP_EXPR || TREE_CODE(fmt_arg) == ADDR_EXPR)
        fmt_arg = TREE_OPERAND(fmt_arg, 0);
    
    if (TREE_CODE(fmt_arg) == STRING_CST) {
        const char *str = TREE_STRING_POINTER(fmt_arg);
        const char *p = str;
        int arg_count = call_expr_nargs(call) - 1 - fmt_idx;
        int next_arg = 0;
        int max_arg_used = -1;

        while (*p) {
            if (*p == '{') {
                p++;
                if (*p == '{') { p++; continue; }
                
                // Replacement field: [arg_id] ["!" conversion] [":" format_spec]
                int arg_id = -1;
                if (ISDIGIT(*p)) {
                    arg_id = 0;
                    while (ISDIGIT(*p)) {
                        arg_id = arg_id * 10 + (*p - '0');
                        p++;
                    }
                }

                int current_arg;
                if (arg_id != -1) {
                    current_arg = arg_id;
                } else {
                    current_arg = next_arg++;
                }
                if (current_arg > max_arg_used) max_arg_used = current_arg;

                if (*p == '!') {
                    p++;
                    if (*p == 'a') p++;
                    else warning_at(loc, 0, "invalid conversion specifier %qc", *p);
                }

                if (*p == ':') {
                    p++;
                    // Basic parsing of standard_format_spec
                    // [[fill]align][sign]["#"]["0"][width][grouping][.precision]["L"][type]
                    
                    // Fill and Align
                    if (*p && strchr("<>=^", p[1])) {
                        p += 2;
                    } else if (*p && strchr("<>=^", *p)) {
                        p++;
                    }

                    // Sign
                    if (*p && strchr("+- ", *p)) p++;

                    // #
                    if (*p == '#') p++;

                    // 0
                    if (*p == '0') p++;

                    // Width
                    if (*p == '{') {
                        p++;
                        int w_arg_id = -1;
                        if (ISDIGIT(*p)) {
                            w_arg_id = 0;
                            while (ISDIGIT(*p)) { w_arg_id = w_arg_id * 10 + (*p - '0'); p++; }
                        }
                        int w_arg = (w_arg_id != -1) ? w_arg_id : next_arg++;
                        if (w_arg > max_arg_used) max_arg_used = w_arg;
                        if (w_arg < arg_count) {
                            tree arg = CALL_EXPR_ARG(call, fmt_idx + 1 + w_arg);
                            if (!is_integer_type(TREE_TYPE(arg))) {
                                warning_at(loc, 0, "dynamic width argument %d must be an integer", w_arg);
                            }
                        }
                        if (*p == '}') p++;
                        else warning_at(loc, 0, "unclosed dynamic width field");
                    } else if (ISDIGIT(*p)) {
                        while (ISDIGIT(*p)) p++;
                    }

                    // Grouping
                    if (*p == '_' || *p == ',') p++;

                    // Precision
                    if (*p == '.') {
                        p++;
                        if (*p == '{') {
                            p++;
                            int p_arg_id = -1;
                            if (ISDIGIT(*p)) {
                                p_arg_id = 0;
                                while (ISDIGIT(*p)) { p_arg_id = p_arg_id * 10 + (*p - '0'); p++; }
                            }
                            int p_arg = (p_arg_id != -1) ? p_arg_id : next_arg++;
                            if (p_arg > max_arg_used) max_arg_used = p_arg;
                            if (p_arg < arg_count) {
                                tree arg = CALL_EXPR_ARG(call, fmt_idx + 1 + p_arg);
                                if (!is_integer_type(TREE_TYPE(arg))) {
                                    warning_at(loc, 0, "dynamic precision argument %d must be an integer", p_arg);
                                }
                            }
                            if (*p == '}') p++;
                            else warning_at(loc, 0, "unclosed dynamic precision field");
                        } else if (ISDIGIT(*p)) {
                            while (ISDIGIT(*p)) p++;
                        }
                    }

                    // L
                    if (*p == 'L') p++;

                    // Type
                    char type_char = 0;
                    if (*p && strchr("aAbBcdeEfFgGoOsxXnp u", *p)) {
                        type_char = *p;
                        p++;
                    }

                    // Check type compatibility
                    if (current_arg < arg_count) {
                        tree arg = CALL_EXPR_ARG(call, fmt_idx + 1 + current_arg);
                        tree type = TREE_TYPE(arg);
                        if (type && TREE_CODE(type) == BOOLEAN_TYPE) {
                            if (type_char && !strchr("sbBduoxX", type_char)) {
                                warning_at(loc, 0, "invalid format specifier %qc for %<bool%>", type_char);
                            }
                        }
                    }

                    // If we are not at '}', then something is wrong with the spec
                    if (*p && *p != '}') {
                        warning_at(loc, 0, "invalid format specification %qs", p);
                        // Skip to next '}'
                        while (*p && *p != '}') p++;
                    }
                }

                if (*p == '}') {
                    p++;
                } else {
                    warning_at(loc, 0, "unclosed replacement field");
                }
            } else if (*p == '}') {
                p++;
                if (*p == '}') { p++; continue; }
                warning_at(loc, 0, "unmatched %<}%> in format string");
            } else {
                p++;
            }
        }

        int expected = max_arg_used + 1;
        if (arg_count < expected) {
            warning_at(loc, 0, "too few arguments for yio format string (expected %d, got %d)", expected, arg_count);
        } else if (arg_count > expected) {
            // Only warn if there were actually some format specifiers
            if (expected > 0 || strchr(str, '{')) {
                warning_at(loc, 0, "too many arguments for yio format string (expected %d, got %d)", expected, arg_count);
            }
        }
    }
}

static tree walk_tree_callback(tree *tp, int *walk_subtrees, void *data) {
    tree t = *tp;
    if (TREE_CODE(t) == CALL_EXPR) {
        tree fn = CALL_EXPR_FN(t);
        while (TREE_CODE(fn) == ADDR_EXPR || TREE_CODE(fn) == NOP_EXPR) fn = TREE_OPERAND(fn, 0);
        if (TREE_CODE(fn) != FUNCTION_DECL) return NULL_TREE;
        
        int fmt_idx = get_fmt_index(fn);
        if (fmt_idx >= 0) {
            validate_yio_call(EXPR_LOCATION(t), t, fmt_idx);
        }
    }
    return NULL_TREE;
}

static void plugin_pre_genericize(void *event_data, void *data) {
    tree fndecl = (tree)event_data;
    walk_tree(&DECL_SAVED_TREE(fndecl), walk_tree_callback, NULL, NULL);
}

static tree handle_yio_format_attribute(tree *node, tree name, tree args, int flags, bool *no_add_attrs) {
    return NULL_TREE;
}

static struct attribute_spec yio_format_attr = {
    "yio_format", 1, 1, false, false, false, false,
    handle_yio_format_attribute, NULL
};

static void register_attributes(void *event_data, void *data) {
    register_attribute(&yio_format_attr);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
    if (!plugin_default_version_check(version, &gcc_version)) return 1;
    register_callback(plugin_info->base_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
    register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE, plugin_pre_genericize, NULL);
    return 0;
}
