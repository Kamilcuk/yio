# SDCC _Generic Support

**Version:** 4.5.0 #15242 (Linux)
**Targets:** mcs51, stm8, z80, mos6502, pdk14, hc08
**Consistency:** Behavior is identical across all listed targets.
**Mandatory Flags:** `--model-large --stack-auto --less-pedantic` (and other project defaults).

## Type Resolution Rules

SDCC `_Generic` merges types by stripping specific attributes. Including multiple types from the same "Bucket" category in one association list triggers **Error 228**.

| Bucket Category | Attributes Stripped | Example |
| :--- | :--- | :--- |
| **Qualifiers** | `const`, `volatile` | `const char*` resolves to `char*` |
| **Pointers vs Arrays** | Array size / extent | `char[7]` resolves to `char*` |
| **Address Space (Partial)**| `__code` | `__code char*` matches `char*` |

## Association Mapping for String Literals (`"abc"`)

A string literal in SDCC matches **both** `char*` and `__code char*`. 

| Association Type | Result | Note |
| :--- | :--- | :--- |
| `char*` | **Match** | Default generic pointer. |
| `__code char*` | **Match** | Specific address space. |
| `__xdata char*` | **No Match** | Literals do not reside in XDATA. |
| `char*` AND `__code char*` | **Error 228** | Ambigious; literals resolve to both. |

## Supported Distinct Type Associations

The following are recognized as distinct types (safe to use together in `_Generic`):

- **Standard Scalars:** `char`, `int`, `long`, `long long`, `float`.
- **Signedness:** `char*` and `unsigned char*` are distinct.
- **Pointers:** Pointers to different base types (e.g., `char*` vs `int*`) are distinct.
- **SDCC Specific (8051):** `__bit`, `__sfr`.

## Unsupported Keywords
The following are NOT recognized as types or keywords (Fixed-Point TR 18037):
- `_Accum`, `_Fract`, `_Sat`.

## STM8 Resolution Rules (Internal Types)

For STM8 with `--model-large`, SDCC uses internal pointer types that exhibit complex overlapping behavior in `_Generic`. While they are distinct classes, using more than one in an association list often triggers **Error 228**.

| Pointer Class | Internal Name | Size | Input Example | Matching Association |
| :--- | :--- | :--- | :--- | :--- |
| **Generic** | `generic*` | 3 bytes | `char *p` | `char *` |
| **Near (Data)**| `near*` | 2 bytes | `&arr[0]` (local array) | `__typeof(&((char *)0)[0])` |
| **Code** | `__code*` | 3 bytes | `&"abc"[0]` or `&main` | `__typeof(&("")[0])` |

### Observed Overlap and Error 228
Empirical testing on SDCC 4.5.0 shows that these types **clash** in `_Generic`.
- **Generic vs. Code:** `char *` and `__typeof(&("")[0])` resolve to the same bucket. Including both triggers **Error 228**.
- **Generic Match:** A generic pointer (`char *p`) satisfies **all three** associations individually.
- **Literal Match:** A string literal (`"abc"`) matches both `char *` and `__typeof(&("")[0])`.
- **Array Match:** A character array (`char arr[2]`) **only** matches `__typeof(&((char *)0)[0])`. It does **not** decay to a generic `char *` in `_Generic`.

### Recommended Association Strategy
To avoid Error 228 while supporting all inputs (pointers, literals, and arrays), use a single association that covers the widest range:
- Use `char *` to catch both generic pointers and string literals.
- For arrays, use `&arr[0]` at the call site. This forces the array to decay into a generic pointer type that matches the `char *` association.
- **NEVER** include both `char *` and the `__typeof` variants in the same `_Generic` list, as they clash for generic pointer types.


### Qualifiers and Storage Classes
- **Const Merging:** `const char *` and `char *` resolve to the same bucket and cannot be used together in one `_Generic` (Error 228).
- **Internal Suffixes:** Error messages may append `fixed` (global/static) or `auto` (stack) to types (e.g., `char near* auto`). These suffixes indicate linkage/storage and are ignored by `_Generic` matching logic.

## Error 228: Ambiguity Example
```c
_Generic(p, 
    char*: 1, 
    const char*: 2 // ERROR 228: Resolves to the same bucket (char*)
)
```
