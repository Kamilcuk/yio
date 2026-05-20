#ifndef FSTRING_UTILS_HPP
#define FSTRING_UTILS_HPP

#include "fstring.hpp"
#include <stdarg.h>

static inline void report_fstring_problem(location_t loc, const char *msg, ...) {
  if (!warn_fstring_format && !error_fstring_format) { return; }

  auto_diagnostic_group d;
  va_list ap;
  va_start(ap, msg);
  if (error_fstring_format) {
    emit_diagnostic_valist(DK_ERROR, loc, -1, msg, &ap);
  } else {
    emit_diagnostic_valist(DK_WARNING, loc, 0, msg, &ap);
  }
  va_end(ap);
}

static inline std::string format_integer(tree val, const struct yio_printfmt_s &spec) {
  YIO_buf buf;
  YIO_buf_init(&buf);

  int err = 0;
  unsigned __int128 uval = 0;
  bool is_neg = false;

  if (TYPE_UNSIGNED(TREE_TYPE(val))) {
    uval = (unsigned __int128)(unsigned HOST_WIDE_INT)TREE_INT_CST_ELT(val, 0);
    if (TREE_INT_CST_NUNITS(val) > 1) {
      uval |= ((unsigned __int128)(unsigned HOST_WIDE_INT)TREE_INT_CST_ELT(val, 1)) << HOST_BITS_PER_WIDE_INT;
    }
  } else {
    widest_int w = wi::to_widest(val);
    if (wi::neg_p(w)) {
      is_neg = true;
      w = -w;
    }
    uval = (unsigned __int128)w.to_uhwi();
    if (HOST_BITS_PER_WIDE_INT < 128 && w.get_precision() > HOST_BITS_PER_WIDE_INT) {
        if (w.get_len() > 1) {
            uval |= ((unsigned __int128)(unsigned HOST_WIDE_INT)w.elt(1)) << HOST_BITS_PER_WIDE_INT;
        }
    }
  }

#if YIO_HAS_INT128
  err = YIO_buf_print_u128_in(&buf, spec, uval);
  if (!err && is_neg) {
      YIO_buf_clear(&buf);
      yio_printctx_t ctx = {0};
      ctx.pf = spec;
      ctx.out = YIO_buf_yprintf_cb;
      ctx.outarg = &buf;
      err = YIO_print_uint128_in(&ctx, uval, true);
  }
#else
  err = YIO_buf_print_ull_in(&buf, spec, (unsigned long long)uval);
  if (!err && is_neg) {
      YIO_buf_clear(&buf);
      yio_printctx_t ctx = {0};
      ctx.pf = spec;
      ctx.out = YIO_buf_yprintf_cb;
      ctx.outarg = &buf;
      err = YIO_print_ullong_in(&ctx, (unsigned long long)uval, true);
  }
#endif

  std::string result;
  if (!err) {
    result.assign(YIO_buf_data(&buf), YIO_buf_len(&buf));
  }
  YIO_buf_fini(&buf);
  return result;
}

static inline tree get_string_cst(tree t) {
  if (!t) { return NULL_TREE; }
  while (CONVERT_EXPR_P(t) || TREE_CODE(t) == NON_LVALUE_EXPR) t = TREE_OPERAND(t, 0);
  if (TREE_CODE(t) == ADDR_EXPR) { t = TREE_OPERAND(t, 0); }
  while (CONVERT_EXPR_P(t) || TREE_CODE(t) == NON_LVALUE_EXPR || TREE_CODE(t) == ARRAY_REF) t = TREE_OPERAND(t, 0);
  if (TREE_CODE(t) == STRING_CST) { return t; }
  return NULL_TREE;
}

static inline bool is_optimizable_type(tree type) {
  if (!type) return false;
  type = TYPE_MAIN_VARIANT(type);
  if (INTEGRAL_TYPE_P(type) || SCALAR_FLOAT_TYPE_P(type))
    return true;
  if (POINTER_TYPE_P(type)) {
    tree target = TREE_TYPE(type);
    if (!target) return true; // void*
    target = TYPE_MAIN_VARIANT(target);
    if (target == char_type_node || target == void_type_node)
      return true;
  }
  return false;
}

static inline tree find_var_recursive(tree block, tree id) {
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

static inline tree find_var_in_scope(tree fndecl, const char *name) {
  tree id = get_identifier(name);
  tree decl = lookup_name(id);
  if (decl && (TREE_CODE(decl) == VAR_DECL || TREE_CODE(decl) == PARM_DECL)) { return decl; }
  for (tree arg = DECL_ARGUMENTS(fndecl); arg; arg = DECL_CHAIN(arg)) {
    if (DECL_NAME(arg) == id) { return arg; }
  }
  return find_var_recursive(DECL_INITIAL(fndecl), id);
}

#endif // FSTRING_UTILS_HPP
