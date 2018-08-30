#include <stdio.h>
#include "capitest.h"

int main()
{
    capitest_TestSuite suite = capitest_TestSuite_INIT;
    if (register_test_tuple(&suite) < 0) {
        printf("failed to register test_tuple\n");
        return 1;
    }
    int exitcode = capitest_testsuite_run(&suite);
    return exitcode;
}
