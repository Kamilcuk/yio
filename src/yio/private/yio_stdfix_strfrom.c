/**
 * @file
 * @date 2020-sie-04
 * @author Kamil Cukrowski
 * @copyright
 * SPDX-License-Identifier: GPL-3.0-only
 * @brief
 */

/**
 * @brief Formats a fixed-point number stored in an unsigned integer into a string buffer.
 * @param precision0     The original precision requested by the user, or default if not set.
 * @param precision      The effective precision to use for formatting.
 * @param spec           The format specifier (e.g., 'f', 'a', 'x'). Always lowercase.
 * @param spec_is_upper  True if the original format specifier was uppercase (e.g., 'F', 'A', 'X').
 * @param o              The output string buffer to write characters to.
 * @param v              The unsigned integer representation of the absolute value of the fixed-point number.
 * @param ibit           The number of integral bits in the fixed-point type.
 * @param fbit           The number of fractional bits in the fixed-point type.
 * @return               0 on success, or an error code if the string operation fails.
 */
#include "yio_stdfix_strfrom.h"
#include "private.h"
#include "yio_string.h"
#include "yio_stdfix.h"
#include <stdint.h>
#include <limits.h>
#include <assert.h>
#include <ctype.h>

#if YYIO_HAS_STDFIX_TYPES

{% from 'yio/private/yio_stdfix.h' import j_STDFIX %}

static const char YYIO_stdfix_strfrom_i_to_c_HEX[] = "0123456789ABCDEF";
static const char YYIO_stdfix_strfrom_i_to_c_hex[] = "0123456789abcdef";
static inline
const char *YYIO_stdfix_strfrom_i_to_c(bool upper) {
	return upper ? YYIO_stdfix_strfrom_i_to_c_HEX : YYIO_stdfix_strfrom_i_to_c_hex;
}

// Represents the type we will use to represnt stdfix types as an unsigned integer.
// WIDTH is defined to a 8, 16, 32, 64 outside of this macro.
#define TYPE  YYIO_XCONCAT(YYIO_XCONCAT(uint_least, WIDTH), _t)

{% call(V) j_FOREACHAPPLY([8, 16, 32, 64]) %}
#line

#define WIDTH  $1

#if $1 == 64
#if YYIO_HAS_INT128
#define TYPEX2   unsigned __int128
#endif
#else
// Represents twice as bigger as TYPE type.
#define TYPEX2   uint_least{{V*2}}_t
#endif

typedef struct {
    TYPE remainder;
    uint8_t digit;
} yyio_next_digit_$1_t;

static inline yyio_next_digit_$1_t yyio_get_next_digit_$1(TYPE rem, int fbit, TYPE mask) {
#ifdef TYPEX2
  // Case A: We have a double-width accumulator (e.g., 64-bit for 32-bit type)
  TYPEX2 full = (TYPEX2)rem * 10;
  return (yyio_next_digit_$1_t){ full & mask, full >> fbit };
#elif $1 == 64
  // Case B: No wider type available (usually 64-bit CPU without __int128)
  // Use the 32-bit split-multiply logic for 64-bit values
  uint64_t hi = rem >> 32;
  uint64_t lo = rem & 0xFFFFFFFFULL;
  uint64_t p_lo = lo * 10;
  uint64_t p_hi = hi * 10 + (p_lo >> 32);
  uint64_t res_rem = ((p_hi << 32) | (p_lo & 0xFFFFFFFFULL)) & mask;
  uint8_t res_digit = p_hi >> (fbit >= 32 ? fbit - 32 : 0);
  if (fbit < 32) {
      res_digit = (p_hi << (32 - fbit)) | ((p_lo & 0xFFFFFFFFULL) >> fbit);
  }
  return (yyio_next_digit_$1_t){ res_rem, res_digit };
#else
#error
#endif
}

static inline
int YYIO_stdfix_strfrom_int$1(YYIO_string *o, const struct yio_printfmt_s *pf, char spec, bool spec_is_upper, TYPE v, unsigned ibit, unsigned fbit) {
	const int total_bits = sizeof(v) * CHAR_BIT;
	const char *const i_to_c = YYIO_stdfix_strfrom_i_to_c(spec_is_upper);
	const bool is_pure_fraction = fbit >= total_bits;
	const bool alternate_form = pf->hash;
	int err = 0;
	//
	if (spec == 'x') {
		err = YYIO_string_yprintf(o, "{:x}", v + 0);
		if (err) return err;
	} else if (spec == 'u') {
		err = YYIO_string_yprintf(o, "{:u}", v + 0);
		if (err) return err;
	} else if (spec == 'd') {
		err = YYIO_string_yprintf(o, "{:d}", v + 0);
		if (err) return err;

	} else if (spec == 'f' || spec == 'g') {
		// Default precision for f is 6.
		const size_t precision = yio_precision_get_default(pf->precision, 6);
		// Integer part extraction
		// If it's a pure fraction, integer part is 0.
		// Otherwise, we shift to get the whole number.
		TYPE integer_part = is_pure_fraction ? 0 : (v >> (fbit % total_bits));
		// Masking for the fractional part
		// If it's a pure fraction, the mask covers the whole variable (all 1s).
		// Otherwise, we create a mask for the lower 'fbit' bits.
		const TYPE mask = is_pure_fraction ? (TYPE)-1 : (((TYPE)1 << (fbit % total_bits)) - 1);
		const TYPE fractional_bits = v & mask;
		// Buffer size perfectly matched to the type's bit-depth (e.g., ~21 bytes for 64-bit)
		// Using YYIO_LOG10_POW2 ensures the array is a compile-time constant.
		uint8_t digits[YYIO_LOG10_POW2(sizeof(TYPE) * 8) + 2];
		// Define the "Resolution Wall"
		// This is the max number of meaningful decimal digits the binary type can store.
		const int physical_limit = sizeof(digits) - 2;
		// Determine calculation boundary
		// We only calculate digits that exist in both the user's request AND the buffer.
		const int calc_limit = (precision < physical_limit) ? precision : physical_limit;
		// The Math Loop (Calculates digits from bits)
		if (calc_limit > 0) {
				TYPE fract_rem = fractional_bits;
				// Calculate all printable digits
				for (int i = 0; i < calc_limit; ++i) {
						yyio_next_digit_$1_t res = yyio_get_next_digit_$1(fract_rem, fbit, mask);
    				digits[i] = res.digit;
    				fract_rem = res.remainder;
				}
				// "Rounding Pass" (The Look-ahead) (Nearest Neighbor)
			  yyio_next_digit_$1_t peek = yyio_get_next_digit_$1(fract_rem, fbit, mask);
    		bool carry = peek.digit >= 5;
    		for (int j = calc_limit - 1; j >= 0 && carry; --j) {
        		if (++digits[j] < 10) {
            		carry = false;
        		} else {
            		digits[j] = 0;
            		carry = true;
        		}
    		}
    		if (carry) integer_part++;
		} else {
    	// Case: precision is 0. We still check if we round the integer.
    	// e.g., 0.6 becomes 1
    	yyio_next_digit_$1_t peek = yyio_get_next_digit_$1(fractional_bits, fbit, mask);
    	if (peek.digit >= 5) integer_part++;
		}
		// Print the integer part using the full width of the type.
		// The + 0 promotes integer_part to an integer type, so it isn't detected as `char`.
    err = YYIO_string_yprintf(o, "{}", integer_part + 0);
    if (err) return err;
    // Calculate actual precision for 'g' (strip trailing zeros)
    int effective_precision = calc_limit;
    if (spec == 'g') {
      while (effective_precision > 0 && digits[effective_precision - 1] == 0) {
        effective_precision--;
      }
    } else {
      effective_precision = precision;
    }
    // Print Decimal Point
    if (effective_precision > 0 || alternate_form) {
        err = YYIO_string_putc(o, '.');
        if (err) return err;
    }
    // Print Fractional Part
    if (effective_precision > 0) {
      // Print significant digits
      const int to_print = (effective_precision < calc_limit) ? effective_precision : calc_limit;
      for (int i = 0; i < to_print; ++i) {
          err = YYIO_string_putc(o, i_to_c[digits[i]]);
          if (err) return err;
      }
      // Zero-padding for 'f' only
      if (spec == 'f' && effective_precision > calc_limit) {
        for (int i = calc_limit; i < effective_precision; ++i) {
            err = YYIO_string_putc(o, '0');
            if (err) return err;
        }
      }
    }

	} else if (spec == 'a') {
		err = YYIO_string_putc(o, '0');
		if (err) return err;
		err = YYIO_string_putc(o, spec_is_upper ? 'X' : 'x');
		int exponent = 0;
		size_t precision;
		if (v != 0) {
			// Normalizacja do lewej krawędzi
			while (!(v >> (total_bits - 1))) {
				v <<= 1;
				--exponent;
			}
			exponent += ibit - 4;
			precision = yio_precision_get_default(pf->precision, (size_t)(total_bits / 4 - 1));
		} else {
			precision = yio_precision_get_default(pf->precision, 0);
		}
		const int digits_to_print = precision + 1;
		// Correct Rounding: find the bit just below the last nibble we will print
    // Since v is normalized to the top, the last bit of the last printed nibble
    // is at (total_bits - digits_to_print * 4). The guard bit is one below that.
    int guard_bit_pos = total_bits - (digits_to_print * 4) - 1;
    if (guard_bit_pos >= 0 && v != 0) {
        TYPE old_v = v;
        v += ((TYPE)1 << guard_bit_pos);
        // Handle carry out: if v wrapped around, shift right and adjust exponent
        if (v < old_v) {
            v = ((TYPE)1 << (total_bits - 1)); // Set MSB
            exponent += 4; // Carrying a whole nibble in hex is rare but possible
        }
    }
		for (int i = 0; i < digits_to_print; ++i) {
			// Stop if precision wasn't set and we hit trailing zeros
			if (pf->precision == 0 && v == 0 && i > 0) {
				break;
			}
			const int c = v >> (total_bits - 4);
			v <<= 4;
			// Print the lead digit
      err = YYIO_string_putc(o, i_to_c[c]);
      if (err) return err;
      // Print the dot after the FIRST nibble
      if (i == 0 && (precision > 0 || alternate_form)) {
          err = YYIO_string_putc(o, '.');
          if (err) return err;
      }
		}
		err = YYIO_string_putc(o, spec_is_upper ? 'P' : 'p');
		if (err) return err;
		err = YYIO_string_yprintf(o, "{:+d}", exponent + 0);
		if (err) return err;

	} else {
		return YIO_ERROR_ENOSYS;
	}
	return err;
}

#undef WIDTH
#ifdef TYPEX2
#undef TYPEX2
#endif
{% endcall %}

{% call(V) j_FOREACHAPPLY(j_STDFIX) %}
#line
// Represents the number of bits in $2.
#define BITS      ( $3_FBIT + $3_IBIT + {{0 if j_match(V.2, "unsigned") else 1}} )
#if BITS <= 8
// Represents the number of bits aligned to CHAR_BIT.
#define WIDTH    8
#elif BITS <= 16
#define WIDTH    16
#elif BITS <= 32
#define WIDTH    32
#elif BITS <= 64
#define WIDTH    64
#else
#error
#endif

int YYIO_astrfrom$1(YYIO_string *o, const struct yio_printfmt_s *pf, $2 val) {
	_Static_assert(CHAR_BIT == 8, "");
	// Dispatching each type to the same size of variable.
	// After removing negative numbers and in twos-complement representation we do not really care.
	TYPE uint_val = 0;
	_Static_assert(sizeof(val) <= sizeof(uint_val), "");
	memcpy(&uint_val, &val, sizeof(val));
	const char spec = pf->type ? tolower((unsigned char)pf->type) : 'f';
	const bool spec_is_upper = pf->type ? isupper((unsigned char)pf->type) : false;
	{% if not j_match(V.2, "unsigned") %}
	if (uint_val & ((TYPE)1 << (BITS - 1))) {
		if (spec != 'x' && spec != 'u') {
			int err = YYIO_string_putc(o, '-');
			if (err) return err;
			uint_val = -uint_val;
		}
	}
	{% endif %}
	return YYIO_XCONCAT(YYIO_stdfix_strfrom_int, WIDTH)(o, pf, spec, spec_is_upper, uint_val, YYIO_$3_IBIT, YYIO_$3_FBIT);
}

#undef BITS
#undef WIDTH
{% endcall %}

#endif // YYIO_HAS_STDFIX_TYPES
