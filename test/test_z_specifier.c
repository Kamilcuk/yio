#include <yio.h>
#include <yio_test.h>
#include <math.h>

int main() {
#if YIO_HAS_FLOATd
    double val = -0.0;
    
    // Test that without 'z', we get negative zero
    YIO_TEST_EQ("-0.00", "{:.2f}", val);
    
    // Test that with 'z', we get positive zero
    YIO_TEST_EQ("0.00", "{:z.2f}", val);
    
    // Test scientific notation
    YIO_TEST_EQ("-0.00e+00", "{:.2e}", val);
    YIO_TEST_EQ("0.00e+00", "{:z.2e}", val);
    
    // Test small negative value that rounds to zero
    double small_neg = -0.001;
    YIO_TEST_EQ("-0.0", "{:.1f}", small_neg);
    YIO_TEST_EQ("0.0", "{:z.1f}", small_neg);

    // Test positive zero remains positive
    double pos_zero = 0.0;
    YIO_TEST_EQ("0.00", "{:z.2f}", pos_zero);

    // Test negative non-zero remains negative
    double neg_val = -1.23;
    YIO_TEST_EQ("-1.23", "{:z.2f}", neg_val);

    // Test 'g' specifier
    YIO_TEST_EQ("-0", "{:.1g}", val);
    YIO_TEST_EQ("0", "{:z.1g}", val);

    // Test that z coersion works with different precisions
    YIO_TEST_EQ("0.000", "{:z.3f}", -0.0);
    YIO_TEST_EQ("0", "{:z.0f}", -0.0);

#endif

    return 0;
}
