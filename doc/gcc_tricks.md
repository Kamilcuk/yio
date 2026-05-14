# GCC Array Dimension Detection

Detects the level and number of dimensions of an arbitrary array or non-array variable using GCC extensions.

## Implementation

```c
#include <stddef.h>

/*
 * IS_ARRAY(x)
 * Returns 1 if x is an array, 0 if it is a pointer or scalar.
 * Uses GCC extensions: __builtin_classify_type and __builtin_types_compatible_p.
 * Array-to-pointer decay happens with '+ 0'.
 */
#define IS_ARRAY(x) (__builtin_classify_type(x) == 5 && !__builtin_types_compatible_p(typeof(x), typeof((x)+0)))

/*
 * FIRST_EL(x)
 * Safely returns the first element of x if it's an array, or a char if it's not.
 * Used for recursive dimension detection.
 */
#define FIRST_EL(x) (*__builtin_choose_expr(IS_ARRAY(x), (x)+0, (char*)0))

/*
 * DIM_VAL(x)
 * Returns the number of elements in the first dimension of x, or 0 if not an array.
 */
#define DIM_VAL(x) __builtin_choose_expr(IS_ARRAY(x), sizeof(x) / sizeof(FIRST_EL(x)), (size_t)0)

/*
 * ARRAY_DIMENTIONS(x)
 * Returns a compound literal array containing up to 4 dimensions.
 */
#define ARRAY_DIMENTIONS(x) \
    (size_t[]){ \
        DIM_VAL(x), \
        DIM_VAL(FIRST_EL(x)), \
        DIM_VAL(FIRST_EL(FIRST_EL(x))), \
        DIM_VAL(FIRST_EL(FIRST_EL(FIRST_EL(x)))) \
    }
```

## Usage

```c
int a3[4][3][2];
const size_t *dims = ARRAY_DIMENTIONS(a3);
// dims[0] == 4, dims[1] == 3, dims[2] == 2, dims[3] == 0
```

## How it works

1.  **`__builtin_classify_type(x) == 5`**: Identifies that the expression is either an array or a pointer.
2.  **`__builtin_types_compatible_p(typeof(x), typeof((x)+0))`**: Differentiates between array and pointer. An array decays to a pointer when `+ 0` is applied, so if the types are compatible, it was already a pointer.
3.  **`__builtin_choose_expr`**: Essential for recursion. It ensures that the compiler only evaluates the branch that makes sense for the type, preventing "subscripted value is not an array" errors when checking deeper dimensions of a scalar or a 1D array.
4.  **Compound Literal**: `(size_t[]){...}` allows returning a pointer to a temporary array containing the results.

## Safe WRAP_ARRAY for all types

A macro that wraps an array into a `struct arr` but leaves scalars and structs unchanged, without causing compilation errors.
#include <stdint.h>
#include <stddef.h>

struct arr {
    const void *ptr;
    size_t count;
};

/* Safe detection for arrays (1 if array, 0 otherwise) */
#define IS_ARRAY(x) \
    (__builtin_classify_type(x) == 5 && \
     !__builtin_types_compatible_p(typeof(x), \
         typeof(&__builtin_choose_expr(__builtin_classify_type(x) == 5, (x), (char[1]){0})[0])))

/* Safe element size (returns 1 for non-arrays to avoid division by zero) */
#define ELEM_SIZE(x) \
    sizeof(__builtin_choose_expr(IS_ARRAY(x), (x), (char[1]){0})[0])

/* Safe wrapper: returns struct arr or x. 
   Works for arrays, literals (rvalues), and structs. */
#define WRAP_ARRAY(x) \
    __builtin_choose_expr(IS_ARRAY(x), \
        ((struct arr){ \
            (const void *)(uintptr_t)__builtin_choose_expr(IS_ARRAY(x), (x), 0), \
            sizeof(x) / ELEM_SIZE(x) \
        }), \
        (x))
```

### Key Techniques

1.  **Lazy Indexing**: `__builtin_choose_expr` is used to provide a dummy array `(char[1]){0}` when `x` is a struct or scalar. This allows the compiler to see a valid `[0]` index even if it's on a "dead" branch of a built-in.
2.  **Double Hiding**: To handle structs, we use nested `__builtin_choose_expr`. The inner one provides a `0` (integer) when `x` is a struct, satisfying the `(uintptr_t)` cast. When `x` is an array, it provides `(x)` which decays to a pointer, also satisfying the cast.

3.  **Literal Support**: Because literals like `5` are never arrays (`IS_ARRAY` is false), they safely fall into the `(x)` branch, avoiding any illegal pointer operations.
3.  **Type-Safe Dispatch**: Since `WRAP_ARRAY` changes the result type based on `x`, it is best used with `_Generic` or variadic functions.
