include(CheckCCompilerFlag)

# Standard development flags for GCC/Clang.
# These are applied as a batch to avoid excessive feature checks.
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	set(dev_flags
		-ggdb3
		-O
		-Wall
		-Wextra
		-Wwrite-strings
		-Wno-unused-function
		-Wno-unused-parameter
		-Werror=vla
		-ffunction-sections
		-fdata-sections
		-fdiagnostics-color=always
		# Hardening / Security flags
		-D_FORTIFY_SOURCE=2
		-D_GLIBCXX_ASSERTIONS
	)
	
	# Architecture-specific optimizations and hardening
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i386|aarch64")
		list(APPEND dev_flags 
			-march=native
			-fstack-clash-protection
			-fcf-protection
			-fstack-protector-all
			-fstack-protector-strong
		)
	endif()

	# GCC-specific hardening and debug info switches
	if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
		list(APPEND dev_flags 
			-fasynchronous-unwind-tables
			-fexceptions
			-grecord-gcc-switches
		)
	endif()

	# Join the list into a single string for CMAKE_C_FLAGS
	string(REPLACE ";" " " dev_flags_str "${dev_flags}")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${dev_flags_str}")
endif()

# GCC Specific Safety Checks (Analyzer and Sanitizers)
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	# GCC Static Analyzer - ALWAYS ENABLED ON GNU
	check_c_compiler_flag("-fanalyzer" HAS_ANALYZER)
	if(HAS_ANALYZER)
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fanalyzer")
	else()
		message(WARNING "yio: GCC Static Analyzer (-fanalyzer) not supported by this GCC version.")
	endif()

	# Sanitizers (ASan, UBSan) - OPTIONAL TOGGLE
	option(YIO_ENABLE_SANITIZERS "Enable sanitizers (ASan, UBSan)" OFF)
	if(YIO_ENABLE_SANITIZERS)
		# Attempt to enable both Address and Undefined sanitizers.
		check_c_compiler_flag("-fsanitize=address,undefined" HAS_SANITIZERS)
		if(HAS_SANITIZERS)
			set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,undefined -fno-omit-frame-pointer")
		else()
			# Try UBSan only if full combo fails (e.g. Alpine/musl).
			check_c_compiler_flag("-fsanitize=undefined" HAS_UBSAN)
			if(HAS_UBSAN)
				set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
			else()
				message(WARNING "yio: No sanitizers (ASan/UBSan) could be enabled for this GCC compiler.")
			endif()
		endif()
	endif()
endif()

# Linker flags for POSIX systems.
# Enables garbage collection of unused sections and various hardening features.
if(NOT APPLE AND NOT CMAKE_C_COMPILER_ID STREQUAL "SDCC")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -Wl,-z,defs -Wl,-z,now")
	if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64|i386|aarch64")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-z,relro")
	endif()
endif()

###############################################################################
# Development Environment Setup

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

if(NOT CMAKE_BUILD_TYPE MATCHES "^Debug$|^Release$|^RelWithDebInfo$")
	message(FATAL_ERROR "You mixed up CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
endif()

# Use ccache if available
find_program(CCACHE_COMMAND "ccache")
if(CCACHE_COMMAND)
	set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_COMMAND}")
endif()

# Create symlink from gen to GENDIR for development purposes
execute_process(
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${GENDIR} ${CMAKE_CURRENT_SOURCE_DIR}/gen
)

# Create symlink to compile_commands.json for IDE to pick it up
execute_process(
	COMMAND ${CMAKE_COMMAND} -E create_symlink
		${CMAKE_BINARY_DIR}/compile_commands.json
		${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
)

yio_debug_variables(
	BUILD_TESTING
	YIO_BUILD_TESTING
	CMAKE_TOOLCHAIN_FILE
	CMAKE_CROSSCOMPILING
	CMAKE_CROSSCOMPILING_EMULATOR
	CMAKE_BINARY_DIR
	CMAKE_CURRENT_BINARY_DIR
	CMAKE_SOURCE_DIR
	CMAKE_CURRENT_SOURCE_DIR
	CMAKE_BUILD_TYPE
	CMAKE_BUILD_TYPE_UPPER
	CMAKE_SYSTEM_NAME
	CMAKE_SYSTEM_PROCESSOR
	CMAKE_C_COMPILER
	CMAKE_C_COMPILER_ID
	CMAKE_C_COMPILER_VERSION
	CMAKE_C_FLAGS
	CMAKE_C_FLAGS_${CMAKE_BUILD_TYPE_UPPER}
	CMAKE_EXE_LINKER_FLAGS
	CMAKE_EXE_LINKER_FLAGS_${CMAKE_BUILD_TYPE_UPPER}
	CMAKE_GENERATOR
	CMAKE_VERBOSE_MAKEFILE
	CMAKE_EXPORT_COMPILE_COMMANDS
	CCACHE_COMMAND
	UNISTRING_LIB
	DIRECTORY:COMPILE_OPTIONS
	DIRECTORY:COMPILE_DEFINITIONS
	DIRECTORY:LINK_OPTIONS
)
