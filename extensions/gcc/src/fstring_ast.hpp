#ifndef FSTRING_AST_HPP
#define FSTRING_AST_HPP

#include "fstring_parser.hpp"

// Forward declaration from GIMPLE
static inline tree create_handler_array(location_t loc, tree elem_type, const std::vector<tree> &handlers);

struct VarResolver {
  tree fndecl;
  location_t loc;
  std::vector<tree> &vars;

  tree resolve_expression(const std::string &name) {
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
    return var;
  }

  tree resolve(const std::string &name, std::string &out_fmt) {
    if (name.empty()) {
      out_fmt += "{}";
      return NULL_TREE;
    }
    tree var = resolve_expression(name);
    if (var) {
      out_fmt += "{}";
      vars.push_back(var);
      return var;
    } else {
      report_fstring_problem(loc, "variable or literal %qs not found for f-string", name.c_str());
      return NULL_TREE;
    }
  }
};

static inline void check_negative_constant(tree val, location_t loc, const char *name) {
  if (val && TREE_CODE(val) == INTEGER_CST && (long)TREE_INT_CST_LOW(val) < 0) {
    report_fstring_problem(loc, "%s cannot be negative", name);
  }
}

static inline void append_reconstructed_spec(std::string &new_fmt, const FStringToken &tok, VarResolver &resolver, tree type) {
  if (tok.spec.empty() && !tok.width && !tok.precision) { return; }
  new_fmt += ":";

  if (type && !is_optimizable_type(type)) {
    new_fmt += tok.spec;
    return;
  }

  if (tok.fill != '\0' || tok.align != '\0') {
    if (tok.fill != '\0') new_fmt += tok.fill;
    if (tok.align != '\0') new_fmt += tok.align;
  }
  if (tok.sign != '\0') new_fmt += tok.sign;
  if (tok.z) { new_fmt += "z"; }
  if (tok.hash) { new_fmt += "#"; }
  if (tok.zero) { new_fmt += "0"; }

  if (tok.width) {
    tree w_tree = resolver.resolve(*tok.width, new_fmt);
    if (w_tree) check_negative_constant(w_tree, resolver.loc, "width");
  } else {
    const char *p = tok.spec.c_str();
    if (tok.fill != '\0' || tok.align != '\0') {
      if (tok.fill != '\0') p++;
      if (tok.align != '\0') p++;
    }
    while (*p && strchr("+- z#0", *p)) { p++; }
    while (*p && ISDIGIT(*p)) { new_fmt += *p++; }
  }

  if (tok.grouping_option != '\0') new_fmt += tok.grouping_option;

  if (tok.precision) {
    new_fmt += ".";
    tree p_tree = resolver.resolve(*tok.precision, new_fmt);
    if (p_tree) check_negative_constant(p_tree, resolver.loc, "precision");
  } else {
    const char *p = strchr(tok.spec.c_str(), '.');
    if (p) {
      new_fmt += ".";
      p++;
      while (*p && ISDIGIT(*p)) { new_fmt += *p++; }
    }
  }

  if (tok.locale) { new_fmt += "L"; }
  if (tok.type != '\0') { new_fmt += tok.type; }
}

static inline tree find_fstring_call(tree t) {
  if (!t) { return NULL_TREE; }
  while (CONVERT_EXPR_P(t) || TREE_CODE(t) == NON_LVALUE_EXPR) t = TREE_OPERAND(t, 0);
  if (TREE_CODE(t) == CALL_EXPR) {
    tree fn = CALL_EXPR_FN(t);
    while (CONVERT_EXPR_P(fn) || TREE_CODE(fn) == NON_LVALUE_EXPR) fn = TREE_OPERAND(fn, 0);
    if (TREE_CODE(fn) == ADDR_EXPR) { fn = TREE_OPERAND(fn, 0); }
    if (fn && TREE_CODE(fn) == FUNCTION_DECL && DECL_NAME(fn) &&
        strcmp(IDENTIFIER_POINTER(DECL_NAME(fn)), "__builtin_fstring") == 0) {
      return t;
    }
  }
  return NULL_TREE;
}

static inline tree get_fstring_elem_type(tree call, location_t loc, int nargs) {
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
    if (elem_type == NULL_TREE) { elem_type = h_type; }
    else if (elem_type != h_type) { report_fstring_problem(loc, "all handlers must have the same type in %<__builtin_fstring%> (mismatched %qT vs %qT). ensure all handler functions have the same signature", h_type, elem_type); }
  }
  if (elem_type) { if (TREE_CODE(elem_type) == FUNCTION_TYPE) { elem_type = build_pointer_type(elem_type); } }
  else { elem_type = ptr_type_node; }
  return elem_type;
}

static inline tree find_handler_for_type(tree type, const std::map<tree, tree> &handlers) {
  if (!type) return NULL_TREE;
  tree variant = TYPE_MAIN_VARIANT(type);
  auto it = handlers.find(variant);
  return (it != handlers.end()) ? it->second : NULL_TREE;
}

static inline std::map<tree, tree> parse_call_handlers(tree call, location_t loc) {
  std::map<tree, tree> call_handlers;
  int nargs = call_expr_nargs(call);
  for (int i = 1; i + 1 < nargs; i += 2) {
    tree t_ptr = CALL_EXPR_ARG(call, i);
    tree h_fn = CALL_EXPR_ARG(call, i + 1);
    STRIP_NOPS(t_ptr);
    tree type = TREE_TYPE(t_ptr);
    if (type && TREE_CODE(type) == POINTER_TYPE) { type = TREE_TYPE(type); }
    if (type) {
      type = TYPE_MAIN_VARIANT(type);
      if (VOID_TYPE_P(type)) {
        report_fstring_problem(loc, "cannot use void as a target type for handler in %<__builtin_fstring%>. did you mean %<void *%>? use %<(void **)0%> instead");
      }
      if (call_handlers.count(type)) {
        report_fstring_problem(loc, "duplicate handler for type %qT. remove the redundant handler registration", type);
      }
      call_handlers[type] = h_fn;
    }
  }
  return call_handlers;
}

static inline void expand_fstring_builtin(tree call, tree fndecl, std::vector<tree> &out_args) {
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

  std::map<tree, tree> call_handlers = parse_call_handlers(call, loc);
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

      tree var = resolver.resolve_expression(tok.expr);
      if (!var) {
        report_fstring_problem(loc, "variable or literal %qs not found for f-string", tok.expr.c_str());
        new_fmt += "{";
        new_fmt += tok.expr;
        append_reconstructed_spec(new_fmt, tok, resolver, NULL_TREE);
        new_fmt += "}";
        continue;
      }

      tree type = TREE_TYPE(var);
      validate_type_spec(type, tok, loc);

      new_fmt += "{";
      vars.push_back(var);
      append_reconstructed_spec(new_fmt, tok, resolver, type);
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
    out_args.push_back(v);
  }
}

static inline bool is_fstring_format_call(tree t, int &fmt_idx) {
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

static inline void validate_fstring_format(tree call, int fmt_idx) {
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
      validate_type_spec(TREE_TYPE(arg), tok, loc);
    }

    if (tok.width) {
      width_count++;
      int w_idx = get_idx(*tok.width, "width");
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

    if (tok.precision) {
      precision_count++;
      int p_idx = get_idx(*tok.precision, "precision");
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

static inline tree rebuild_call(tree old_call, const std::vector<tree> &new_args) {
  location_t loc = EXPR_LOCATION(old_call);
  vec<tree, va_gc> *v;
  vec_alloc(v, new_args.size());
  for (tree a : new_args) { v->quick_push(a); }

  auto_vec<location_t> arg_locs;
  for (tree a : new_args) {
    location_t aloc = EXPR_LOCATION(a);
    if (aloc == UNKNOWN_LOCATION) aloc = loc;
    arg_locs.safe_push(aloc);
  }

  return build_function_call_vec(loc, arg_locs.to_vec_legacy(), CALL_EXPR_FN(old_call), v, NULL);
}

static inline bool is_empty_string_literal(tree t) {
  tree s = get_string_cst(t);
  return s && TREE_STRING_LENGTH(s) == 1 && TREE_STRING_POINTER(s)[0] == '\0';
}

static inline tree walk_tree_callback(tree *tp, int *walk_subtrees, void *data) {
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
            }
          }

          if (has_magic_string) {
            for (tree a : expanded) {
              new_args.push_back(a);
            }
            modified = true;
            i++; // skip ""
            continue;
          } else {
            location_t next_loc = (i + 1 < nargs) ? EXPR_LOCATION(CALL_EXPR_ARG(t, i + 1)) : EXPR_LOCATION(t);
            report_fstring_problem(next_loc, "argument following nested f-string must be an empty string literal");
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

#endif // FSTRING_AST_HPP
