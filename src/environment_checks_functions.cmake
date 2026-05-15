# environment_checks_functions.cmake

# This script checks for available functions and runtime behavior.
# It should be included after third_party libraries so that it can
# correctly detect symbols in provided libraries (like libdfp).

if(TARGET dfp)
	list(APPEND CMAKE_REQUIRED_LIBRARIES
		dfp
	)
endif()

yio_config_gen_check_symbol_exists(asprintf "stdio.h" YIO_HAS_asprintf LANGUAGE C)
yio_config_gen_check_symbol_exists(strnlen "string.h" YIO_HAS_strnlen LANGUAGE C)

if(YIO_HAS_WCHAR_H)
	yio_config_gen_check_symbol_exists(wcwidth "wchar.h" YIO_HAS_wcwidth LANGUAGE C)
	yio_config_gen_check_symbol_exists(wcsnrtombs "wchar.h" YIO_HAS_wcsnrtombs LANGUAGE C)
else()
	yio_config_gen_add_value(YIO_HAS_wcwidth 0)
	yio_config_gen_add_value(YIO_HAS_wcsnrtombs 0)
endif()

#########################################################################
# handle and detect _floats (function checks)

foreach(ii IN LISTS _floats)
	foreach_count_items(ii foreachstatevar
		type mathsuffix suffix strtosuffix
	)
	if(ii)
		continue()
	endif()

	if(YIO_HAS_FLOAT${suffix})
		# Bulk check for all functions
		set(_src "
#define _GNU_SOURCE
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
int main() {
	volatile ${type} f = 0, f2 = 0; volatile int i; char buf[1];
	f = exp2${mathsuffix}(f);
	f = exp10${mathsuffix}(f);
	f = floor${mathsuffix}(f);
	f = fabs${mathsuffix}(f);
	f = log10${mathsuffix}(f);
	f = log2${mathsuffix}(f);
	f = frexp${mathsuffix}(f, (int*)&i);
	f = modf${mathsuffix}(f, (${type}*)&f2);
	f = pow${mathsuffix}(f, f2);
	f = nextafter${mathsuffix}(f, f2);
	strfrom${suffix}(buf, 1, \"%g\", f);
	f = strto${strtosuffix}(\"0\", NULL);
	return 0;
}")
		# Use a unique variable for bulk check to avoid caching issues between types
		if(TARGET dfp AND suffix MATCHES "^d")
			cmake_push_check_state()
			list(APPEND CMAKE_REQUIRED_LIBRARIES dfp)
			check_c_source_compiles("${_src}" YIO_HAS_BULK_${suffix})
			cmake_pop_check_state()
		else()
			check_c_source_compiles("${_src}" YIO_HAS_BULK_${suffix})
		endif()

		if(YIO_HAS_BULK_${suffix})
			foreach(func IN ITEMS exp2 exp10 floor fabs log10 log2 frexp modf pow nextafter strfrom strto)
				set(YIO_HAS_${func}${suffix} 1 CACHE INTERNAL "")
				yio_config_gen_add(YIO_HAS_${func}${suffix})
			endforeach()
		else()
			# Fallback to individual checks if bulk fails
			check_symbol_exists_bool(pow${mathsuffix} "math.h" YIO_HAS_pow${suffix})
			if(YIO_HAS_pow${suffix})
				foreach(func IN ITEMS exp2 exp10 floor fabs log10 log2 frexp modf nextafter)
					check_symbol_exists_bool(${func}${mathsuffix} "math.h" YIO_HAS_${func}${suffix})
					yio_config_gen_add(YIO_HAS_${func}${suffix})
				endforeach()
				yio_config_gen_add(YIO_HAS_pow${suffix})
				if(TARGET dfp AND suffix MATCHES "^d")
					cmake_push_check_state()
					list(APPEND CMAKE_REQUIRED_LIBRARIES dfp)
					check_symbol_exists_bool(strfrom${suffix}    "stdlib.h"  YIO_HAS_strfrom${suffix})
					check_symbol_exists_bool(strto${strtosuffix} "stdlib.h"  YIO_HAS_strto${suffix})
					cmake_pop_check_state()
				else()
					check_symbol_exists_bool(strfrom${suffix}    "stdlib.h"  YIO_HAS_strfrom${suffix})
					check_symbol_exists_bool(strto${strtosuffix} "stdlib.h"  YIO_HAS_strto${suffix})
				endif()
				foreach(ii IN ITEMS strfrom strto)
					yio_config_gen_add(YIO_HAS_${ii}${suffix})
				endforeach()
			else()
				foreach(func IN ITEMS exp2 exp10 floor fabs log10 log2 frexp modf pow nextafter strfrom strto)
					yio_config_gen_add_value(YIO_HAS_${func}${suffix} 0)
				endforeach()
			endif()
		endif()
	endif()
endforeach()

if(NOT DEFINED YIO_MUSL_BROKEN_EXP10)
	if(NOT CMAKE_CROSSCOMPILING AND YIO_HAS_exp10l AND YIO_HAS_FLOAT_H)
		set(_o  ${CMAKE_CURRENT_BINARY_DIR}/checkexp10)
		file(MAKE_DIRECTORY ${_o})
		file(WRITE ${_o}/checkexp10.c [=[
#include <math.h>
int main() {
	return powl(10.0, -4933) == 0;
}
]=]
		)
		try_run(
			runres compileres
			${_o} ${_o}/checkexp10.c
			LINK_LIBRARIES m
		)
		if(compileres AND runres EQUAL 1)
			set(YIO_MUSL_BROKEN_EXP10 1 CACHE INTERNAL "")
		endif()
	endif()
endif()
if(NOT DEFINED YIO_MUSL_BROKEN_EXP10)
	set(YIO_MUSL_BROKEN_EXP10 0 CACHE INTERNAL "")
endif()
yio_config_gen_add(YIO_MUSL_BROKEN_EXP10)

yio_config_gen_fini()
