#include "conftest.h"
#include <stdint.h>
#include <stdio.h>

static int h_int(ftest_ctx *ctx) {
  if (ctx) {
    printf("h_int: %d\n", *(int *)ctx);
  } else {
    printf("h_int called\n");
  }
  return 0;
}

static int h_uint64(ftest_ctx *ctx) {
  if (ctx) {
    printf("h_uint64: %lu\n", *(uint64_t *)ctx);
  } else {
    printf("h_uint64 called\n");
  }
  return 0;
}

#define MY_HANDLERS (int *)0, h_int, (uint64_t *)0, h_uint64

static void test_call(ftest_handler handlers[], const char *fmt, ...) {
  printf("fmt: %s\n", fmt);
  for (int i = 0; handlers[i]; i++) {
    handlers[i](NULL);
  }
}

static void *get_handlers_ptr(ftest_handler handlers[], const char *fmt, ...) {
  return (void *)handlers;
}
#define get_ctx_ptr(str) get_handlers_ptr(test_f(str, MY_HANDLERS))

int main() {
  int x = 42;
  uint64_t y = 123456789;

  // 1. Basic test
  printf("--- Basic ---\n");
  test_call(test_f("x is {x}, y is {y}", MY_HANDLERS));

  // 2. Literal test
  printf("--- Literal ---\n");
  test_call(test_f("val is {5}", MY_HANDLERS));

  // 3. Deduplication test
  printf("--- Dedupe ---\n");
  void *p1 = get_ctx_ptr("{x}");
  void *p2 = get_ctx_ptr(" {x} "); // Different fmt, same handler
  int a = 1, b = 2;
  void *p3 = get_ctx_ptr("{a}");
  void *p4 = get_ctx_ptr("{b}");
  void *p5 = get_ctx_ptr("{x}, {y}");
  void *p6 = get_ctx_ptr("{a}, {y}");

  printf("p1 == p2: %s\n", p1 == p2 ? "YES" : "NO");
  printf("p3 == p4: %s\n", p3 == p4 ? "YES" : "NO");
  printf("p1 == p3: %s\n", p1 == p3 ? "YES" : "NO");
  printf("p5 == p6: %s\n", p5 == p6 ? "YES" : "NO");

  // 4. Recursive scope lookup
  printf("--- Scope ---\n");
  {
    int inner_var = 100;
    { test_call(test_f("inner: {inner_var}", MY_HANDLERS)); }
  }

  // 5. Constant baking
  printf("--- Baking ---\n");
  const int width = 10;
  test_call(test_f("val: {x:{width}d}", MY_HANDLERS));

  // 6. Dynamic width (non-const)
  printf("--- Dynamic ---\n");
  int dynamic_w = 20;
  test_call(test_f("val: {x:{dynamic_w}d}", MY_HANDLERS));

  return 0;
}

// test_PASS_REGULAR_EXPRESSION fmt: x is {}, y is {}
// test_PASS_REGULAR_EXPRESSION h_int called
// test_PASS_REGULAR_EXPRESSION h_uint64 called
// test_PASS_REGULAR_EXPRESSION fmt: val is {}
// test_PASS_REGULAR_EXPRESSION p1 == p2: YES
// test_PASS_REGULAR_EXPRESSION p3 == p4: YES
// test_PASS_REGULAR_EXPRESSION p1 == p3: YES
// test_PASS_REGULAR_EXPRESSION p5 == p6: YES
// test_PASS_REGULAR_EXPRESSION fmt: inner: {}
// test_PASS_REGULAR_EXPRESSION fmt: val: {:10d}
// test_PASS_REGULAR_EXPRESSION fmt: val: {:{}d}
