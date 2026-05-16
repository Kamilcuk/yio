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

static void validate_type_spec(tree type, std::string spec, location_t loc) {
  if (spec.empty()) { return; }
  char type_char = '\0';
  char last = spec.back();
  if (ISALPHA(last)) { type_char = last; }

  if (INTEGRAL_TYPE_P(type)) {
    const char *integral_specs = "duoxbDUOXBc";
    if (type_char != '\0' && !strchr(integral_specs, type_char)) {
      report_fstring_problem(loc, "invalid format specifier %qc for %qT. use one of %qs instead", type_char, type, integral_specs);
    }
  } else if (SCALAR_FLOAT_TYPE_P(type)) {
    const char *float_specs = "fegafEGA";
    if (type_char != '\0' && !strchr(float_specs, type_char)) {
      report_fstring_problem(loc, "invalid format specifier %qc for %qT. use one of %qs instead", type_char, type, float_specs);
    }
  }
}

class FStringParser {
public:
  struct Result {
    std::string new_fmt;
    std::vector<tree> vars;
  };

  FStringParser(tree fndecl, location_t loc) : fndecl(fndecl), loc(loc) {}

  Result parse(const char *str) {
    Result res;
    const char *p = str;
    while (*p) {
      if (*p == '{') {
        p++;
        if (*p == '{') {
          res.new_fmt += "{{";
          p++;
        } else {
          parse_replacement_field(res, p);
        }
      } else if (*p == '}') {
        p++;
        if (*p == '}') {
          res.new_fmt += "}}";
          p++;
        } else {
          report_fstring_problem(loc, "unmatched %<}%> in f-string. remove it or escape as %<}}%>");
        }
      } else {
        res.new_fmt += *p++;
      }
    }
    return res;
  }

private:
  tree fndecl;
  location_t loc;

  void parse_replacement_field(Result &res, const char *&p) {
    res.new_fmt += "{";
    std::string var_name;
    while (*p && *p != '}' && *p != ':' && *p != '!') {
      if (*p == '{') { break; }
      var_name += *p++;
    }
    if (var_name.empty()) {
      if (*p == '}' || *p == ':' || *p == '!') {
        report_fstring_problem(loc, "empty expression in f-string. provide a variable or literal inside %<{...}%>");
      }
    } else {
      tree var = find_var_in_scope(fndecl, var_name.c_str());
      if (!var) {
        char *endptr;
        long val = strtol(var_name.c_str(), &endptr, 0);
        if (*endptr == '\0') { var = build_int_cst(integer_type_node, val); }
      }
      if (var) {
        res.vars.push_back(var);
      } else {
        report_fstring_problem(
            loc, "variable or literal %qs not found for f-string. ensure it is declared in the current scope", var_name.c_str()
        );
      }
    }
    if (*p == '!') {
      res.new_fmt += *p++;
      while (*p && *p != ':' && *p != '}') {
        res.new_fmt += *p++;
      }
    }
    std::string spec;
    if (*p == ':') {
      res.new_fmt += *p++;
      while (*p && *p != '}') {
        if (*p == '{') {
          p++;
          if (*p == '{') {
            res.new_fmt += "{{";
            p++;
            continue;
          }
          std::string inner_var;
          while (*p && *p != '}' && *p != ':') {
            inner_var += *p++;
          }
          if (*p == ':') {
            report_fstring_problem(loc, "nested format specifier not allowed in dynamic field. remove the inner %<:%>");
            while (*p && *p != '}') { p++; }
            if (*p == '}') { p++; }
            return;
          }
          if (*p == '}') { p++; }
          tree var = find_var_in_scope(fndecl, inner_var.c_str());
          if (var && TREE_CODE(var) == VAR_DECL && TREE_READONLY(var) && DECL_INITIAL(var) &&
              TREE_CODE(DECL_INITIAL(var)) == INTEGER_CST) {
            std::string val_str = std::to_string(TREE_INT_CST_LOW(DECL_INITIAL(var)));
            res.new_fmt += val_str;
            spec += val_str;
          } else {
            res.new_fmt += "{}";
            spec += "{}";
            if (var) {
              res.vars.push_back(var);
            } else {
              char *endptr;
              long val = strtol(inner_var.c_str(), &endptr, 0);
              if (*endptr == '\0') {
                std::string val_str = std::to_string(val);
                res.new_fmt.replace(res.new_fmt.length() - 2, 2, val_str);
                spec.replace(spec.length() - 2, 2, val_str);
              } else {
                report_fstring_problem(
                    loc, "variable %qs not found for dynamic field. ensure it is declared in the current scope", inner_var.c_str()
                );
              }
            }
          }
        } else {
          res.new_fmt += *p;
          spec += *p++;
        }
      }
    }
    if (!res.vars.empty() && !spec.empty()) {
      validate_type_spec(TREE_TYPE(res.vars.back()), spec, loc);
    }
    if (*p == '}') {
      res.new_fmt += "}";
      p++;
    } else {
      report_fstring_problem(loc, "unclosed %<{%> in f-string. add a matching %<}%> or escape as %<{{%>");
    }
  }
};

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

  FStringParser parser(fndecl, loc);
  FStringParser::Result fr = parser.parse(merged_str.c_str());

  std::vector<tree> selected_handlers;
  for (tree var : fr.vars) {
    tree var_type = TYPE_MAIN_VARIANT(TREE_TYPE(var));
    tree selected_handler = NULL_TREE;
    for (auto const &pair : call_handlers) {
      if (var_type == pair.first) {
        selected_handler = pair.second;
        break;
      }
    }
    if (!selected_handler) {
      report_fstring_problem(loc, "no handler found for type %qT in f-string. register a handler for this type in the %<__builtin_fstring%> call", var_type);
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

  tree decl;
  static int counter = 0;
  char name[64];
  sprintf(name, "__yio_fstring_arr_%d", counter++);
  
  vec<constructor_elt, va_gc> *elts = NULL;
  for (tree h : selected_handlers) {
    CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, h);
  }
  CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, build_int_cst(elem_type, 0));
  
  tree array_type =
      build_array_type(elem_type, build_index_type(size_int(selected_handlers.size())));
  
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

  out_args.push_back(build1(ADDR_EXPR, build_pointer_type(elem_type), decl));
  out_args.push_back(build_string_literal(fr.new_fmt.length() + 1, fr.new_fmt.c_str()));
  for (tree var : fr.vars) {
    out_args.push_back(var);
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
  int placeholders = 0;
  int arg_offset = fmt_idx + 1;
  const char *p = fmt;
  while (*p) {
    if (*p == '{') {
      p++;
      if (*p == '{') {
        p++;
        continue;
      }
      const char *spec_start = NULL;
      while (*p && *p != '}' && *p != ':') {
        p++;
      }
      if (*p == ':') {
        spec_start = p;
        p++;
        while (*p && *p != '}') {
          if (*p == '{') {
            const char *inner_start = ++p;
            if (*p == '{') {
              p++;
              continue;
            }
            while (*p && *p != '}' && *p != ':') {
              p++;
            }
            if (*p == ':') {
              report_fstring_problem(loc, "nested format specifier not allowed in dynamic field. remove the inner %<:%>");
              while (*p && *p != '}') { p++; }
              if (*p == '}') { p++; }
              return;
            }
            if (*p == '}') {
              std::string inner(inner_start, p - inner_start);
              if (!inner.empty()) {
                char *endptr;
                strtol(inner.c_str(), &endptr, 10);
                if (*endptr != '\0') {
                  report_fstring_problem(loc, "invalid dynamic field index %qs. use a numeric index or empty braces", inner.c_str());
                  while (*p && *p != '}') { p++; }
                  if (*p == '}') { p++; }
                  return;
                }
              }

              int arg_idx = arg_offset + placeholders;
              if (arg_idx < nargs) {
                tree arg = CALL_EXPR_ARG(call, arg_idx);
                bool is_precision = false;
                const char *pp = inner_start - 1;
                while (pp > spec_start) {
                  pp--;
                  if (*pp == '.') {
                    is_precision = true;
                    break;
                  }
                  if (*pp == '{' || *pp == ':') { break; }
                  if (*pp != ' ' && *pp != '\t') { break; }
                }

                if (!INTEGRAL_TYPE_P(TREE_TYPE(arg))) {
                  if (is_precision) {
                    report_fstring_problem(
                        loc, "dynamic precision argument %d must be an integer. check the argument corresponding to placeholder %d", placeholders + 1, placeholders + 1
                    );
                  } else {
                    report_fstring_problem(
                        loc, "dynamic width argument %d must be an integer. check the argument corresponding to placeholder %d", placeholders + 1, placeholders + 1
                    );
                  }
                }
              }
              placeholders++;
              p++;
            } else {
              report_fstring_problem(loc, "unclosed replacement field. add a matching %<}%>");
              return;
            }
          } else {
            if (*p == '?') { report_fstring_problem(loc, "invalid format specification. remove unexpected characters"); }
            p++;
          }
        }
      }

      if (*p == '}') {
        int arg_idx = arg_offset + placeholders;
        if (arg_idx < nargs && spec_start) {
          tree arg = CALL_EXPR_ARG(call, arg_idx);
          tree type = TREE_TYPE(arg);
          std::string spec(spec_start, p - spec_start);
          validate_type_spec(type, spec, loc);
        }
        placeholders++;
        p++;
      } else {
        report_fstring_problem(loc, "unclosed replacement field. add a matching %<}%>");
        return;
      }
    } else if (*p == '}') {
      p++;
      if (*p == '}') {
        p++;
        continue;
      }
      report_fstring_problem(loc, "unmatched %<}%>. remove it or escape as %<}}%>");
    } else {
      p++;
    }
  }

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

static tree walk_tree_callback(tree *tp, int *walk_subtrees, void *data) {
  tree t = *tp;
  tree fndecl = (tree)data;

  if (TREE_CODE(t) == CALL_EXPR) {
    tree fcall_standalone = find_fstring_call(t);
    if (fcall_standalone) {
      std::vector<tree> expanded;
      expand_fstring_builtin(fcall_standalone, fndecl, expanded);
      if (!expanded.empty()) {
        tree new_call = build_call_vec(TREE_TYPE(t), CALL_EXPR_FN(t), NULL);
        vec<tree, va_gc> *v;
        vec_alloc(v, expanded.size());
        for (tree a : expanded) { v->quick_push(a); }
        new_call = build_call_vec(TREE_TYPE(t), CALL_EXPR_FN(t), v);
        SET_EXPR_LOCATION(new_call, EXPR_LOCATION(t));
        *tp = new_call;
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
            tree next_arg = get_string_cst(CALL_EXPR_ARG(t, i + 1));
            if (next_arg) {
              if (TREE_STRING_LENGTH(next_arg) == 1 && TREE_STRING_POINTER(next_arg)[0] == '\0') {
                has_magic_string = true;
              } else {
                report_fstring_problem(EXPR_LOCATION(t), "the argument following %<__builtin_fstring%> must be an empty string literal %<\"\"%>, got %qs instead. ensure your f-string macro ends with %<, \"\"%>", TREE_STRING_POINTER(next_arg));
              }
            } else {
              report_fstring_problem(EXPR_LOCATION(t), "the argument following %<__builtin_fstring%> must be an empty string literal %<\"\"%>, got a non-string argument. ensure your f-string macro ends with %<, \"\"%>");
            }
          } else {
            report_fstring_problem(EXPR_LOCATION(t), "missing required empty string literal %<\"\"%> following %<__builtin_fstring%>. ensure your f-string macro ends with %<, \"\"%>");
          }

          if (has_magic_string) {
            for (tree e : expanded) { new_args.push_back(e); }
            modified = true;
            i++; // skip ""
            continue;
          }
        }
      }
      new_args.push_back(arg);
    }
    if (modified) {
      tree new_call = build_call_vec(TREE_TYPE(t), CALL_EXPR_FN(t), NULL);
      vec<tree, va_gc> *v;
      vec_alloc(v, new_args.size());
      for (tree a : new_args) { v->quick_push(a); }
      new_call = build_call_vec(TREE_TYPE(t), CALL_EXPR_FN(t), v);
      SET_EXPR_LOCATION(new_call, EXPR_LOCATION(t));
      *tp = new_call;
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
