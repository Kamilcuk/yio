#ifndef FSTRING_GIMPLE_HPP
#define FSTRING_GIMPLE_HPP

#include "fstring_parser.hpp"

// Utility to follow SSA names to their constant definitions
static tree follow_ssa_to_const(tree t) {
  if (!t) return NULL_TREE;
  while (TREE_CODE(t) == SSA_NAME) {
    gimple *def = SSA_NAME_DEF_STMT(t);
    if (is_gimple_assign(def)) {
      tree_code code = gimple_assign_rhs_code(def);
      if (code == INTEGER_CST || code == REAL_CST || code == ADDR_EXPR) {
        t = gimple_assign_rhs1(def);
      } else break;
    } else break;
  }
  // Strip NOPs after following SSA
  while (CONVERT_EXPR_P(t) || TREE_CODE(t) == NON_LVALUE_EXPR) t = TREE_OPERAND(t, 0);
  return t;
}

static inline tree create_array_decl(location_t loc, tree id, tree array_type, tree ctor) {
  tree decl = build_decl(loc, VAR_DECL, id, array_type);
  TREE_STATIC(decl) = 1;
  TREE_READONLY(decl) = 1;
  TREE_CONSTANT(decl) = 1;
  DECL_INITIAL(decl) = ctor;
  TREE_USED(decl) = 1;
  DECL_CONTEXT(decl) = NULL_TREE;
  DECL_ARTIFICIAL(decl) = 1;
  TREE_PUBLIC(decl) = 0;
  varpool_node::finalize_decl(decl);
  return decl;
}

static inline tree create_handler_array(location_t loc, tree elem_type, const std::vector<tree> &handlers) {
  static int counter = 0;
  char name[64];
  sprintf(name, "__fstring_h_L%d", counter++);

  vec<constructor_elt, va_gc> *elts = NULL;
  for (tree h : handlers) {
    CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, h);
  }
  CONSTRUCTOR_APPEND_ELT(elts, NULL_TREE, build_int_cst(elem_type, 0));

  tree array_type =
      build_array_type(elem_type, build_index_type(size_int(handlers.size())));

  tree decl = create_array_decl(loc, get_identifier(name), array_type, build_constructor(array_type, elts));
  return build1(ADDR_EXPR, build_pointer_type(elem_type), decl);
}

// Pass 1: Constant Folding
const pass_data pass_data_fstring_fold = {
    GIMPLE_PASS, "fstring_fold", OPTGROUP_NONE, TV_NONE,
    PROP_gimple_any, 0, 0, 0, 0,
};

class pass_fstring_fold : public gimple_opt_pass {
public:
  pass_fstring_fold(gcc::context *ctxt) : gimple_opt_pass(pass_data_fstring_fold, ctxt) {}
  virtual bool gate(function *) { return fstring_optimizations; }
  virtual unsigned int execute(function *fun) {
    basic_block bb;
    bool changed = false;
    FOR_EACH_BB_FN(bb, fun) {
      for (gimple_stmt_iterator gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
        gimple *stmt = gsi_stmt(gsi);
        if (!is_gimple_call(stmt)) continue;
        gcall *call = as_a<gcall *>(stmt);
        
        int handlers_idx = -1;
        int nargs = gimple_call_num_args(call);
        for (int i = 0; i < nargs; i++) {
          tree arg = follow_ssa_to_const(gimple_call_arg(call, i));
          if (TREE_CODE(arg) == ADDR_EXPR) {
            tree decl = TREE_OPERAND(arg, 0);
            if (TREE_CODE(decl) == VAR_DECL && DECL_NAME(decl)) {
              const char *name = IDENTIFIER_POINTER(DECL_NAME(decl));
              if (strncmp(name, "__fstring_h_", 12) == 0 || strncmp(name, "__fstring_handlers_", 19) == 0) {
                handlers_idx = i; break;
              }
            }
          }
        }
        if (handlers_idx == -1 || handlers_idx + 1 >= nargs) continue;
        tree fmt_arg = get_string_cst(follow_ssa_to_const(gimple_call_arg(call, handlers_idx + 1)));
        if (!fmt_arg) continue;
        
        tree h_arg = follow_ssa_to_const(gimple_call_arg(call, handlers_idx));
        if (TREE_CODE(h_arg) != ADDR_EXPR) continue;
        tree h_decl = TREE_OPERAND(h_arg, 0);
        tree h_ctor = DECL_INITIAL(h_decl);
        if (!h_ctor || TREE_CODE(h_ctor) != CONSTRUCTOR) continue;
        unsigned int h_len = vec_safe_length(CONSTRUCTOR_ELTS(h_ctor));
        
        const char *fmt_str = TREE_STRING_POINTER(fmt_arg);
        location_t loc = gimple_location(stmt);
        std::vector<FStringToken> tokens = parse_fstring(fmt_str, loc);
        std::string new_fmt_str;
        std::vector<tree> new_args, selected_handlers;
        int arg_offset = handlers_idx + 2, token_arg_idx = 0;
        bool folded_any = false;
        for (auto &tok : tokens) {
          if (tok.is_text) { new_fmt_str += tok.text; continue; }
          if (token_arg_idx >= (int)h_len) break;
          
          int start_idx = token_arg_idx;
          tree val_orig = (arg_offset + token_arg_idx < nargs) ? gimple_call_arg(call, arg_offset + token_arg_idx) : NULL_TREE;
          tree val = follow_ssa_to_const(val_orig);
          tree h = CONSTRUCTOR_ELT(h_ctor, token_arg_idx)->value;
          
          tree w_val = NULL_TREE, p_val = NULL_TREE;
          bool val_foldable = (val && TREE_CODE(val) == INTEGER_CST);
          int next_idx = start_idx + 1;

          if (tok.width) {
            if (next_idx < (int)h_len) {
              tree wv_orig = (arg_offset + next_idx < nargs) ? gimple_call_arg(call, arg_offset + next_idx) : NULL_TREE;
              w_val = follow_ssa_to_const(wv_orig);
              if (!w_val || TREE_CODE(w_val) != INTEGER_CST) val_foldable = false;
              next_idx++;
            } else val_foldable = false;
          }
          if (tok.precision) {
            if (next_idx < (int)h_len) {
              tree pv_orig = (arg_offset + next_idx < nargs) ? gimple_call_arg(call, arg_offset + next_idx) : NULL_TREE;
              p_val = follow_ssa_to_const(pv_orig);
              if (!p_val || TREE_CODE(p_val) != INTEGER_CST) val_foldable = false;
              next_idx++;
            } else val_foldable = false;
          }

          if (val_foldable) {
            struct yio_printfmt_s spec = YIO_printfmt_zero;
            spec.fill = tok.fill; spec.align = tok.align; spec.sign = tok.sign;
            spec.hash = tok.hash; spec.type = tok.type;
            if (tok.zero && spec.align == '\0') { spec.fill = '0'; spec.align = '='; }
            if (tok.width) spec.width = (uint16_t)TREE_INT_CST_LOW(w_val) + 1;
            else { const char *p = tok.spec.c_str(); if (tok.fill || tok.align) { if (tok.fill) p++; if (tok.align) p++; } while (*p && strchr("+- z#0", *p)) p++; if (ISDIGIT(*p)) spec.width = (uint16_t)atoi(p) + 1; }
            if (tok.precision) spec.precision = (uint16_t)TREE_INT_CST_LOW(p_val) + 1;
            else { const char *p = strchr(tok.spec.c_str(), '.'); if (p) spec.precision = (uint16_t)atoi(p + 1) + 1; }
            std::string folded_val = format_integer(val, spec);
            new_fmt_str += folded_val;
            folded_any = true;
            token_arg_idx = next_idx;
          } else {
            int cur_idx = start_idx;
            new_fmt_str += "{";
            if (val_orig) { new_args.push_back(val_orig); selected_handlers.push_back(h); }
            cur_idx++;

            if (tok.spec.empty() && !tok.width && !tok.precision) { 
                new_fmt_str += "}"; 
            } else {
                new_fmt_str += ":";
                if (tok.fill || tok.align) { if (tok.fill) new_fmt_str += tok.fill; if (tok.align) new_fmt_str += tok.align; }
                if (tok.sign) new_fmt_str += tok.sign; if (tok.z) new_fmt_str += 'z'; if (tok.hash) new_fmt_str += '#'; if (tok.zero) new_fmt_str += '0';
                
                const char *s_ptr = tok.spec.c_str();
                if (tok.fill || tok.align) { if (tok.fill) s_ptr++; if (tok.align) s_ptr++; }
                while (*s_ptr && strchr("+- z#0", *s_ptr)) s_ptr++;

                if (tok.width) {
                  tree wv_orig = (arg_offset + cur_idx < nargs) ? gimple_call_arg(call, arg_offset + cur_idx) : NULL_TREE;
                  tree wv = follow_ssa_to_const(wv_orig);
                  tree wh = (cur_idx < (int)h_len) ? CONSTRUCTOR_ELT(h_ctor, cur_idx)->value : NULL_TREE;
                  if (wv && TREE_CODE(wv) == INTEGER_CST) {
                    new_fmt_str += std::to_string(TREE_INT_CST_LOW(wv));
                    folded_any = true;
                  }
                  else if (wv_orig) { new_fmt_str += "{}"; new_args.push_back(wv_orig); selected_handlers.push_back(wh); }
                  cur_idx++;
                  while (*s_ptr && ISDIGIT(*s_ptr)) s_ptr++;
                } else {
                  while (*s_ptr && ISDIGIT(*s_ptr)) new_fmt_str += *s_ptr++;
                }
                
                if (tok.grouping_option) { new_fmt_str += tok.grouping_option; if (*s_ptr == tok.grouping_option) s_ptr++; }
                
                if (tok.precision) {
                  new_fmt_str += ".";
                  if (*s_ptr == '.') s_ptr++;
                  tree pv_orig = (arg_offset + cur_idx < nargs) ? gimple_call_arg(call, arg_offset + cur_idx) : NULL_TREE;
                  tree pv = follow_ssa_to_const(pv_orig);
                  tree ph = (cur_idx < (int)h_len) ? CONSTRUCTOR_ELT(h_ctor, cur_idx)->value : NULL_TREE;
                  if (pv && TREE_CODE(pv) == INTEGER_CST) {
                    new_fmt_str += std::to_string(TREE_INT_CST_LOW(pv));
                    folded_any = true;
                  }
                  else if (pv_orig) { new_fmt_str += "{}"; new_args.push_back(pv_orig); selected_handlers.push_back(ph); }
                  cur_idx++;
                  while (*s_ptr && ISDIGIT(*s_ptr)) s_ptr++;
                } else {
                  const char *dot = strchr(s_ptr, '.');
                  if (dot) {
                      new_fmt_str += "."; s_ptr = dot + 1;
                      while (*s_ptr && ISDIGIT(*s_ptr)) new_fmt_str += *s_ptr++;
                  }
                }
                if (tok.locale) { new_fmt_str += 'L'; if (*s_ptr == 'L') s_ptr++; }
                if (tok.type) new_fmt_str += tok.type;
                new_fmt_str += "}";
            }
            token_arg_idx = next_idx;
          }
        }
        if (folded_any) {
          auto_vec<tree> v;
          for (int i = 0; i < handlers_idx; i++) v.safe_push(gimple_call_arg(call, i));
          tree h_type = TREE_TYPE(TREE_TYPE(h_arg));
          tree elem_type = TREE_TYPE(h_type);
          v.safe_push(create_handler_array(loc, elem_type, selected_handlers));
          v.safe_push(build_string_literal(new_fmt_str.length() + 1, new_fmt_str.c_str()));
          for (tree a : new_args) v.safe_push(a);
          for (int i = arg_offset + token_arg_idx; i < nargs; i++) v.safe_push(gimple_call_arg(call, i));

          gcall *new_c = gimple_build_call_vec(gimple_call_fn(call), v);
          gimple_call_set_lhs(new_c, gimple_call_lhs(call));
          gimple_set_location(new_c, loc);
          gsi_replace(&gsi, new_c, true);
          changed = true;
        }
      }
    }
    return changed ? TODO_update_ssa : 0;
  }
};

// Pass 2: Deduplication (Merging)
const pass_data pass_data_fstring_merge = {
    GIMPLE_PASS, "fstring_merge", OPTGROUP_NONE, TV_NONE,
    PROP_gimple_any, 0, 0, 0, 0,
};

class pass_fstring_merge : public gimple_opt_pass {
public:
  pass_fstring_merge(gcc::context *ctxt) : gimple_opt_pass(pass_data_fstring_merge, ctxt) {}
  virtual bool gate(function *) { return fstring_optimizations; }

  static std::string get_tree_id(tree t) {
    if (!t) return "null";
    STRIP_NOPS(t);
    if (TREE_CODE(t) == ADDR_EXPR) t = TREE_OPERAND(t, 0);
    if (DECL_P(t) && DECL_NAME(t)) return IDENTIFIER_POINTER(DECL_NAME(t));
    if (TREE_CODE(t) == INTEGER_CST) {
      char buf[64];
      if (tree_fits_shwi_p(t)) sprintf(buf, "i%ld", (long)tree_to_shwi(t));
      else sprintf(buf, "u%lu", (unsigned long)tree_to_uhwi(t));
      return buf;
    }
    return "unk";
  }

  static tree find_or_create_global_handler(location_t loc, tree array_decl) {
    tree ctor = DECL_INITIAL(array_decl);
    if (!ctor || TREE_CODE(ctor) != CONSTRUCTOR) return NULL_TREE;
    
    std::string content_id = "__fstring_h_G_";
    unsigned int i; tree val;
    FOR_EACH_CONSTRUCTOR_VALUE(CONSTRUCTOR_ELTS(ctor), i, val) {
      content_id += get_tree_id(val) + "_";
    }

    tree id = get_identifier(content_id.c_str());
    symtab_node *node = symtab_node::get_for_asmname(id);
    if (node && is_a<varpool_node *>(node)) return node->decl;

    tree new_decl = create_array_decl(loc, id, TREE_TYPE(array_decl), unshare_expr(ctor));
    return new_decl;
  }

  virtual unsigned int execute(function *fun) {
    basic_block bb;
    bool changed = false;
    FOR_EACH_BB_FN(bb, fun) {
      for (gimple_stmt_iterator gsi = gsi_start_bb(bb); !gsi_end_p(gsi); gsi_next(&gsi)) {
        gimple *stmt = gsi_stmt(gsi);
        if (!is_gimple_call(stmt)) continue;
        gcall *call = as_a<gcall *>(stmt);
        
        for (int i = 0; i < (int)gimple_call_num_args(call); i++) {
          tree arg = gimple_call_arg(call, i);
          STRIP_NOPS(arg);
          if (TREE_CODE(arg) == ADDR_EXPR) {
            tree decl = TREE_OPERAND(arg, 0);
            if (TREE_CODE(decl) == VAR_DECL && DECL_NAME(decl)) {
              const char *name = IDENTIFIER_POINTER(DECL_NAME(decl));
              if (strncmp(name, "__fstring_h_", 12) == 0 || strncmp(name, "__fstring_handlers_", 19) == 0) {
                tree global_decl = find_or_create_global_handler(gimple_location(stmt), decl);
                if (global_decl && global_decl != decl) {
                  gimple_call_set_arg(call, i, build_fold_addr_expr(global_decl));
                  changed = true;
                }
              }
            }
          }
        }
      }
    }
    return changed ? TODO_update_ssa : 0;
  }
};

#endif // FSTRING_GIMPLE_HPP
