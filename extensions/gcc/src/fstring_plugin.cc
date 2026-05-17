#include "config.h"
#include "system.h"
#include <string>
#include <vector>
#include <map>
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

int plugin_is_GPL_compatible;

static bool warn_fstring_format = true;
static bool error_fstring_format = false;

static void report_fstring_problem(location_t loc, const char *msg, ...) {
  if (!warn_fstring_format && !error_fstring_format) { return; }

  va_list ap;
  va_start(ap, msg);
  if (error_fstring_format) {
    emit_diagnostic_valist(DK_ERROR, loc, -1, msg, &ap);
  } else {
    emit_diagnostic_valist(DK_WARNING, loc, 0, msg, &ap);
  }
  va_end(ap);
}

static tree find_var_recursive(tree block, tree id) {
  if (!block || TREE_CODE(block) != BLOCK) { return NULL_TREE; }
  for (tree var = BLOCK_VARS(block); var; var = DECL_CHAIN(var)) {
    if (DECL_NAME(var) == id) { return var; }
  }
  for (tree sub = BLOCK_SUBBLOCKS(block); sub; sub = BLOCK_CHAIN(sub)) {
    tree found = find_var_recursive(sub, id);
    if (found) { return found; }
  }
  return NULL_TREE;
}

static tree find_var_in_scope(tree fndecl, const char *name) {
  tree id = get_identifier(name);
  tree decl = lookup_name(id);
  if (decl && (TREE_CODE(decl) == VAR_DECL || TREE_CODE(decl) == PARM_DECL)) { return decl; }
  for (tree arg = DECL_ARGUMENTS(fndecl); arg; arg = DECL_CHAIN(arg)) {
    if (DECL_NAME(arg) == id) { return arg; }
  }
  return find_var_recursive(DECL_INITIAL(fndecl), id);
}

struct FStringToken {
  bool is_text;
  std::string text;      // Literal text or full placeholder string
  std::string expr;      // Variable/expression name
  std::string spec;      // Format specifier excluding colon
  std::string width;     // Extracted nested expression for width
  std::string precision; // Extracted nested expression for precision
  bool has_width = false;
  bool has_precision = false;
};

static void validate_type_spec(tree type, std::string spec, location_t loc) {
  if (spec.empty()) { return; }
  const char *p = spec.c_str();

  // Python-style format specifier validation:
  // [[fill]align][sign][z][#][0][width][grouping_option][.precision][type]

  // [[fill]align]
  if (*p && *(p + 1) && strchr("<>=^", *(p + 1))) {
    if (*p == '{' || *p == '}') {
      report_fstring_problem(loc, "brace %qc is not allowed as a fill character", *p);
    }
    p += 2;
  } else if (*p && strchr("<>=^", *p)) {
    p++;
  }

  // [sign][z][#][0]
  while (*p && strchr("+- z#0", *p)) {
    p++;
  }

  // [width]
  while (*p && ISDIGIT(*p)) {
    p++;
  }

  // [grouping_option]
  if (*p && strchr("_,", *p)) {
    p++;
  }

  // [.precision]
  if (*p == '.') {
    p++;
    while (*p && ISDIGIT(*p)) {
      p++;
    }
  }

  char type_char = '\0';
  if (*p && ISALPHA(*p)) {
    type_char = *p;
    p++;
  }

  if (*p != '\0') {
    report_fstring_problem(loc, "invalid format specification");
    return;
  }

  if (type == NULL_TREE) { return; }

  if (INTEGRAL_TYPE_P(type)) {
    const char *integral_specs = "duoxbDUOXBc";
    if (type_char != '\0' && !strchr(integral_specs, type_char)) {
      char tc[2] = {type_char, '\0'};
      report_fstring_problem(loc, "invalid format specifier %s for %qT. use one of %qs instead", tc, type, integral_specs);
    }
  } else if (SCALAR_FLOAT_TYPE_P(type)) {
    const char *float_specs = "fegafEGA";
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

static std::vector<FStringToken> parse_fstring(const char *str, location_t loc) {
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
        tokens.push_back({true, "{", "", "", "", ""});
        p++;
      } else {
        FStringToken tok = {false, "{", "", "", "", ""};
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
                  tok.has_precision = true;
                } else {
                  tok.width = inner;
                  tok.has_width = true;
                }
                tok.text += *p++;
              }
            } else {
              tok.spec += *p;
              tok.text += *p++;
            }
          }
          validate_type_spec(NULL_TREE, tok.spec, loc);
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
        tokens.push_back({true, "}", "", "", "", ""});
        p++;
      } else {
        report_fstring_problem(loc, "unmatched %<}%> in f-string. remove it or escape as %<}}%>");
      }
    } else {
      std::string text;
      while (*p && *p != '{' && *p != '}') { text += *p++; }
      tokens.push_back({true, text, "", "", "", ""});
    }
  }
  return tokens;
}

static tree get_string_cst(tree t) {
  if (!t) { return NULL_TREE; }
  STRIP_NOPS(t);
  if (TREE_CODE(t) == ADDR_EXPR) { t = TREE_OPERAND(t, 0); }
  STRIP_NOPS(t);
  if (TREE_CODE(t) == STRING_CST) { return t; }
  return NULL_TREE;
}

static tree find_fstring_call(tree t) {
  if (!t) { return NULL_TREE; }
  STRIP_NOPS(t);
  if (TREE_CODE(t) == CALL_EXPR) {
    tree fn = CALL_EXPR_FN(t);
    STRIP_NOPS(fn);
    if (TREE_CODE(fn) == ADDR_EXPR) { fn = TREE_OPERAND(fn, 0); }
    if (fn && TREE_CODE(fn) == FUNCTION_DECL && DECL_NAME(fn) &&
        strcmp(IDENTIFIER_POINTER(DECL_NAME(fn)), "__builtin_fstring") == 0) {
      return t;
    }
  }
  return NULL_TREE;
}

static tree get_fstring_elem_type(tree call, location_t loc, int nargs) {
  if (nargs > 1 && nargs % 2 == 0) {
    report_fstring_problem(loc, "missing handler function for last type in %<__builtin_fstring%>. ensure you provide arguments in pairs (type, handler)");
  }

  tree elem_type = NULL_TREE;
  for (int i = 1; i + 1 < nargs; i += 2) {
    tree h_fn = CALL_EXPR_ARG(call, i + 1);
    STRIP_NOPS(h_fn);
    tree raw_h_type = TREE_TYPE(h_fn);
    if (TREE_CODE(raw_h_type) != POINTER_TYPE && TREE_CODE(raw_h_type) != FUNCTION_TYPE) {
      report_fstring_problem(loc, "handler must be a pointer or function in %<__builtin_fstring%> (got %qT). pass a function pointer", raw_h_type);
      continue;
    }

    tree h_type = TYPE_MAIN_VARIANT(raw_h_type);
    if (TREE_CODE(h_type) == POINTER_TYPE) { h_type = TYPE_MAIN_VARIANT(TREE_TYPE(h_type)); }

    if (elem_type == NULL_TREE) {
      elem_type = h_type;
    } else if (elem_type != h_type) {
      report_fstring_problem(loc, "all handlers must have the same type in %<__builtin_fstring%> (mismatched %qT vs %qT). ensure all handler functions have the same signature", h_type, elem_type);
    }
  }

  if (elem_type) {
    if (TREE_CODE(elem_type) == FUNCTION_TYPE) { elem_type = build_pointer_type(elem_type); }
  } else {
    elem_type = ptr_type_node;
  }
  return elem_type;
}

static tree create_handler_array(location_t loc, tree elem_type, const std::vector<tree> &handlers) {
  tree decl;
  static int counter = 0;
  char name[64];
  sprintf(name, "__fstring_handlers_%d", counter++);

  vec<constructor_elt, va_gc> *elts = NULL;
  for (tree h : handlers) {
    CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, h);
  }
  CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, build_int_cst(elem_type, 0));

  tree array_type =
      build_array_type(elem_type, build_index_type(size_int(handlers.size())));

  decl = build_decl(loc, VAR_DECL, get_identifier(name), array_type);
  TREE_STATIC(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  DECL_INITIAL(decl) = build_constructor(array_type, elts);
  TREE_USED(decl) = 1;
  DECL_CONTEXT(decl) = NULL_TREE;
  DECL_ARTIFICIAL(decl) = 1;
  TREE_PUBLIC(decl) = 0;

  varpool_node::finalize_decl(decl);
  return build1(ADDR_EXPR, build_pointer_type(elem_type), decl);
}

static tree find_handler_for_type(tree type, const std::map<tree, tree> &handlers) {
  if (!type) return NULL_TREE;
  tree variant = TYPE_MAIN_VARIANT(type);
  auto it = handlers.find(variant);
  return (it != handlers.end()) ? it->second : NULL_TREE;
}

struct VarResolver {
  tree fndecl;
  location_t loc;
  std::vector<tree> &vars;

  tree resolve(const std::string &name, std::string &out_fmt) {
    if (name.empty()) { return NULL_TREE; }
    tree var = find_var_in_scope(fndecl, name.c_str());
    if (!var) {
      char *endptr;
      long val = strtol(name.c_str(), &endptr, 0);
      if (*endptr == '\0') {
        var = build_int_cst(integer_type_node, val);
      } else {
        double dval = strtod(name.c_str(), &endptr);
        if (*endptr == '\0') {
          REAL_VALUE_TYPE rv;
          real_from_string(&rv, name.c_str());
          var = build_real(double_type_node, rv);
        }
      }
    }
    if (var) {
      tree val_tree = var;
      if (TREE_CODE(var) == VAR_DECL && TREE_READONLY(var) && DECL_INITIAL(var) &&
          TREE_CODE(DECL_INITIAL(var)) == INTEGER_CST) {
        val_tree = DECL_INITIAL(var);
      }

      if (TREE_CODE(val_tree) == INTEGER_CST) {
        out_fmt += std::to_string(TREE_INT_CST_LOW(val_tree));
      } else {
        out_fmt += "{}";
        vars.push_back(var);
      }
      return val_tree;
    } else {
      report_fstring_problem(loc, "variable or literal %qs not found for f-string", name.c_str());
      return NULL_TREE;
    }
  }
};

static void check_negative_constant(tree val, location_t loc, const char *name) {
  if (val && TREE_CODE(val) == INTEGER_CST && (long)TREE_INT_CST_LOW(val) < 0) {
    report_fstring_problem(loc, "%s cannot be negative", name);
  }
}

static void append_reconstructed_spec(std::string &new_fmt, const FStringToken &tok, VarResolver &resolver) {
  if (tok.spec.empty() && !tok.has_width && !tok.has_precision) { return; }
  new_fmt += ":";
  const char *s = tok.spec.c_str();
  const char *p_dot = strchr(s, '.');
  if (tok.has_width) {
    if (p_dot) {
      new_fmt.append(s, p_dot - s);
      tree w_tree = resolver.resolve(tok.width, new_fmt);
      check_negative_constant(w_tree, resolver.loc, "width");
      new_fmt += ".";
      if (tok.has_precision) {
        tree p_tree = resolver.resolve(tok.precision, new_fmt);
        check_negative_constant(p_tree, resolver.loc, "precision");
        new_fmt += (p_dot + 1);
      } else {
        new_fmt += (p_dot + 1);
      }
    } else {
      tree w_tree = resolver.resolve(tok.width, new_fmt);
      check_negative_constant(w_tree, resolver.loc, "width");
      new_fmt += tok.spec;
    }
  } else if (tok.has_precision) {
    if (p_dot) {
      new_fmt.append(s, p_dot - s);
      new_fmt += ".";
      tree p_tree = resolver.resolve(tok.precision, new_fmt);
      check_negative_constant(p_tree, resolver.loc, "precision");
      new_fmt += (p_dot + 1);
    } else {
      new_fmt += tok.spec;
      new_fmt += ".";
      tree p_tree = resolver.resolve(tok.precision, new_fmt);
      check_negative_constant(p_tree, resolver.loc, "precision");
    }
  } else {
    new_fmt += tok.spec;
  }
}

static void expand_fstring_builtin(tree call, tree fndecl, std::vector<tree> &out_args) {
  location_t loc = EXPR_LOCATION(call);
  int nargs = call_expr_nargs(call);
  if (nargs < 1) {
    report_fstring_problem(loc, "too few arguments to %<__builtin_fstring%>. add a format string literal as the first argument");
    return;
  }

  tree str_arg = get_string_cst(CALL_EXPR_ARG(call, 0));
  if (!str_arg) {
    report_fstring_problem(loc, "first argument to %<__builtin_fstring%> must be a string literal. pass a constant string literal instead of a variable");
    return;
  }
  std::string merged_str = TREE_STRING_POINTER(str_arg);

  std::map<tree, tree> call_handlers;
  for (int i = 1; i + 1 < nargs; i += 2) {
    tree t_ptr = CALL_EXPR_ARG(call, i);
    tree h_fn = CALL_EXPR_ARG(call, i + 1);
    STRIP_NOPS(t_ptr);
    tree type = TREE_TYPE(t_ptr);
    if (type && TREE_CODE(type) == POINTER_TYPE) { type = TREE_TYPE(type); }
    if (type) {
      type = TYPE_MAIN_VARIANT(type);
      if (call_handlers.count(type)) {
        report_fstring_problem(loc, "duplicate handler for type %qT. remove the redundant handler registration", type);
      }
      call_handlers[type] = h_fn;
    }
  }

  tree elem_type = get_fstring_elem_type(call, loc, nargs);

  std::vector<FStringToken> tokens = parse_fstring(merged_str.c_str(), loc);
  std::string new_fmt;
  std::vector<tree> vars;
  VarResolver resolver = {fndecl, loc, vars};

  for (const auto &tok : tokens) {
    if (tok.is_text) {
      new_fmt += tok.text;
    } else {
      if (tok.expr.empty()) {
        report_fstring_problem(loc, "empty expression in f-string. provide a variable or literal inside %<{...}%>");
      }
      new_fmt += "{";
      tree var = find_var_in_scope(fndecl, tok.expr.c_str());
      if (!var) {
        char *endptr;
        long val = strtol(tok.expr.c_str(), &endptr, 0);
        if (*endptr == '\0') {
          var = build_int_cst(integer_type_node, val);
        } else {
          double dval = strtod(tok.expr.c_str(), &endptr);
          if (*endptr == '\0') {
            REAL_VALUE_TYPE rv;
            real_from_string(&rv, tok.expr.c_str());
            var = build_real(double_type_node, rv);
          }
        }
      }
      if (var) {
        vars.push_back(var);
        validate_type_spec(TREE_TYPE(var), tok.spec, loc);
      } else {
        report_fstring_problem(loc, "variable or literal %qs not found for f-string", tok.expr.c_str());
      }

      append_reconstructed_spec(new_fmt, tok, resolver);
      new_fmt += "}";
    }
  }

  std::vector<tree> selected_handlers;
  for (tree var : vars) {
    tree selected_handler = find_handler_for_type(TREE_TYPE(var), call_handlers);
    if (!selected_handler) {
      report_fstring_problem(loc, "no handler found for type %qT in f-string. register a handler for this type in the %<__builtin_fstring%> call", TYPE_MAIN_VARIANT(TREE_TYPE(var)));
      selected_handler = build_int_cst(elem_type, 0);
    }
    tree h = selected_handler;
    if (TREE_CODE(TREE_TYPE(h)) == FUNCTION_TYPE) {
      h = build_fold_addr_expr(h);
    } else {
      h = fold_convert(elem_type, h);
    }
    selected_handlers.push_back(h);
  }

  out_args.push_back(create_handler_array(loc, elem_type, selected_handlers));
  out_args.push_back(build_string_literal(new_fmt.length() + 1, new_fmt.c_str()));
  for (tree v : vars) {
    out_args.push_back(default_conversion(v));
  }
}

static bool is_fstring_format_call(tree t, int &fmt_idx) {
  if (TREE_CODE(t) != CALL_EXPR) { return false; }
  tree fn = CALL_EXPR_FN(t);
  STRIP_NOPS(fn);
  if (TREE_CODE(fn) == ADDR_EXPR) { fn = TREE_OPERAND(fn, 0); }
  if (TREE_CODE(fn) != FUNCTION_DECL) { return false; }
  tree attr = lookup_attribute("fstring_format", DECL_ATTRIBUTES(fn));
  if (!attr) { return false; }
  tree args = TREE_VALUE(attr);
  if (args && TREE_CODE(args) == TREE_LIST) {
    tree val = TREE_VALUE(args);
    if (val && TREE_CODE(val) == INTEGER_CST) {
      fmt_idx = (int)TREE_INT_CST_LOW(val) - 1;
      return true;
    }
  }
  return false;
}

static void validate_fstring_format(tree call, int fmt_idx) {
  int nargs = call_expr_nargs(call);
  if (fmt_idx >= nargs) { return; }
  tree fmt_arg = CALL_EXPR_ARG(call, fmt_idx);
  STRIP_NOPS(fmt_arg);
  if (TREE_CODE(fmt_arg) == ADDR_EXPR) {
    tree op0 = TREE_OPERAND(fmt_arg, 0);
    if (TREE_CODE(op0) == STRING_CST) { fmt_arg = op0; }
  }
  if (TREE_CODE(fmt_arg) != STRING_CST) { return; }

  const char *fmt = TREE_STRING_POINTER(fmt_arg);
  location_t loc = EXPR_LOCATION(call);
  int auto_idx = 0;
  int width_count = 0;
  int precision_count = 0;
  int arg_offset = fmt_idx + 1;

  std::vector<FStringToken> tokens = parse_fstring(fmt, loc);
  int max_idx = -1;

  for (const auto &tok : tokens) {
    if (tok.is_text) { continue; }

    auto get_idx = [&](const std::string &expr, const char *field_name) {
      int idx;
      if (expr.empty()) {
        idx = auto_idx++;
      } else {
        char *endptr;
        idx = (int)strtol(expr.c_str(), &endptr, 10);
        if (*endptr != '\0') {
          report_fstring_problem(loc, "invalid dynamic field index %qs. use a numeric index or empty braces for %s", expr.c_str(), field_name);
          return -1;
        }
      }
      if (idx > max_idx) max_idx = idx;
      return idx;
    };

    int main_idx = get_idx(tok.expr, "replacement field");
    if (main_idx >= 0 && arg_offset + main_idx < nargs) {
      tree arg = CALL_EXPR_ARG(call, arg_offset + main_idx);
      validate_type_spec(TREE_TYPE(arg), tok.spec, loc);
    }

    if (tok.has_width) {
      width_count++;
      int w_idx = get_idx(tok.width, "width");
      if (w_idx >= 0 && arg_offset + w_idx < nargs) {
        tree arg = CALL_EXPR_ARG(call, arg_offset + w_idx);
        if (!INTEGRAL_TYPE_P(TREE_TYPE(arg))) {
          report_fstring_problem(loc, "dynamic width argument %d must be an integer", width_count);
        } else {
          STRIP_NOPS(arg);
          check_negative_constant(arg, loc, "width");
        }
      }
    }

    if (tok.has_precision) {
      precision_count++;
      int p_idx = get_idx(tok.precision, "precision");
      if (p_idx >= 0 && arg_offset + p_idx < nargs) {
        tree arg = CALL_EXPR_ARG(call, arg_offset + p_idx);
        if (!INTEGRAL_TYPE_P(TREE_TYPE(arg))) {
          report_fstring_problem(loc, "dynamic precision argument %d must be an integer", precision_count);
        } else {
          STRIP_NOPS(arg);
          check_negative_constant(arg, loc, "precision");
        }
      }
    }
  }

  int placeholders = max_idx + 1;
  int actual_args = nargs - arg_offset;
  if (actual_args < placeholders) {
    report_fstring_problem(
        loc, "too few arguments for format string (expected %d, got %d). provide more arguments after the format string", placeholders, actual_args
    );
  } else if (actual_args > placeholders) {
    report_fstring_problem(
        loc, "too many arguments for format string (expected %d, got %d). remove extra arguments", placeholders, actual_args
    );
  }
}

static tree rebuild_call(tree old_call, const std::vector<tree> &new_args) {
  vec<tree, va_gc> *v;
  vec_alloc(v, new_args.size());
  for (tree a : new_args) { v->quick_push(a); }
  tree new_call = build_call_vec(TREE_TYPE(old_call), CALL_EXPR_FN(old_call), v);
  SET_EXPR_LOCATION(new_call, EXPR_LOCATION(old_call));
  return new_call;
}

static bool is_empty_string_literal(tree t) {
  tree s = get_string_cst(t);
  return s && TREE_STRING_LENGTH(s) == 1 && TREE_STRING_POINTER(s)[0] == '\0';
}

static tree walk_tree_callback(tree *tp, int *walk_subtrees, void *data) {
  tree t = *tp;
  tree fndecl = (tree)data;

  if (TREE_CODE(t) == CALL_EXPR) {
    tree fcall_standalone = find_fstring_call(t);
    if (fcall_standalone) {
      std::vector<tree> expanded;
      expand_fstring_builtin(fcall_standalone, fndecl, expanded);
      if (!expanded.empty()) {
        *tp = rebuild_call(t, expanded);
        *walk_subtrees = 0;
        return NULL_TREE;
      }
    }

    int nargs = call_expr_nargs(t);
    std::vector<tree> new_args;
    bool modified = false;
    for (int i = 0; i < nargs; i++) {
      tree arg = CALL_EXPR_ARG(t, i);
      tree fcall = find_fstring_call(arg);
      if (fcall) {
        std::vector<tree> expanded;
        expand_fstring_builtin(fcall, fndecl, expanded);
        if (!expanded.empty()) {
          bool has_magic_string = false;
          if (i + 1 < nargs) {
            tree next_arg = CALL_EXPR_ARG(t, i + 1);
            if (is_empty_string_literal(next_arg)) {
              has_magic_string = true;
            } else {
              tree s = get_string_cst(next_arg);
              if (s) {
                report_fstring_problem(EXPR_LOCATION(t), "the argument following %<__builtin_fstring%> must be an empty string literal %<\"\"%>, got %qs instead. ensure your f-string macro ends with %<, \"\"%>", TREE_STRING_POINTER(s));
              } else {
                report_fstring_problem(EXPR_LOCATION(t), "the argument following %<__builtin_fstring%> must be an empty string literal %<\"\"%>, got a non-string argument. ensure your f-string macro ends with %<, \"\"%>");
              }
            }
          } else {
            report_fstring_problem(EXPR_LOCATION(t), "missing required empty string literal %<\"\"%> following %<__builtin_fstring%>. ensure your f-string macro ends with %<, \"\"%>");
          }

          if (has_magic_string) {
            for (tree a : expanded) {
              new_args.push_back(default_conversion(a));
            }
            modified = true;
            i++; // skip ""
            continue;
          }
        }
      }
      new_args.push_back(arg);
    }
    if (modified) {
      tree new_call = rebuild_call(t, new_args);
      *tp = new_call;
      int new_fmt_idx;
      if (is_fstring_format_call(new_call, new_fmt_idx)) {
        validate_fstring_format(new_call, new_fmt_idx);
      }
      *walk_subtrees = 0;
      return NULL_TREE;
    }
  }

  int fmt_idx;
  if (is_fstring_format_call(t, fmt_idx)) { validate_fstring_format(t, fmt_idx); }

  return NULL_TREE;
}

static void plugin_pre_genericize(void *event_data, void *data) {
  tree fndecl = (tree)event_data;
  walk_tree_without_duplicates(&DECL_SAVED_TREE(fndecl), walk_tree_callback, fndecl);
}

static struct attribute_spec fstring_format_attr = {
    "fstring_format", 1, 1, false, false, false, false, NULL, NULL};

static void register_attributes(void *event_data, void *data) {
  register_attribute(&fstring_format_attr);
}

int plugin_init(struct plugin_name_args *plugin_info, struct plugin_gcc_version *version) {
  if (!plugin_default_version_check(version, &gcc_version)) { return 1; }

  for (int i = 0; i < plugin_info->argc; i++) {
    if (strcmp(plugin_info->argv[i].key, "Wfstring-format") == 0) {
      if (plugin_info->argv[i].value) { warn_fstring_format = atoi(plugin_info->argv[i].value); }
    } else if (strcmp(plugin_info->argv[i].key, "Werror-fstring-format") == 0) {
      if (plugin_info->argv[i].value) { error_fstring_format = atoi(plugin_info->argv[i].value); }
    }
  }

  register_callback(plugin_info->base_name, PLUGIN_ATTRIBUTES, register_attributes, NULL);
  register_callback(plugin_info->base_name, PLUGIN_PRE_GENERICIZE, plugin_pre_genericize, NULL);
  return 0;
}
