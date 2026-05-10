# environment_floats.cmake

# the list of _floats used for detection and code generation
set(_floats
	#type            mathsuffix suffix    strtosuffix
	"float"          "f"        "f"       "f"
	"double"         ""         "d"       "d"
	"long double"    "l"        "l"       "ld"

	"_Float16"       "f16"      "f16"     "f16"
	"_Float32"       "f32"      "f32"     "f32"
	"_Float64"       "f64"      "f64"     "f64"
	"_Float128"      "f128"     "f128"    "f128"
	"_Float32x"      "f32x"     "f32x"    "f32x"
	"_Float64x"      "f64x"     "f64x"    "f64x"
	"_Float128x"     "f128x"    "f128x"   "f128x"

	"_Decimal32"     "d32"      "d32"     "d32"
	"_Decimal64"     "d64"      "d64"     "d64"
	"_Decimal128"    "d128"     "d128"    "d128"
	"_Decimal32x"    "d32x"     "d32x"    "d32x"
	"_Decimal64x"    "d64x"     "d64x"    "d64x"
	"_Decimal128x"   "d128x"    "d128x"   "d128x"
)
