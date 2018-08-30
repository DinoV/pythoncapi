#include <assert.h>
#include <stdio.h>
#include <Python.h>
#include "capitest.h"


int
capitest_testsuite_add(capitest_TestSuite *suite, const char *name, capitest_TestFunc func)
{
    capitest_Test test = {.name = name, .func = func};
    if (suite->ntest >= Py_ARRAY_LENGTH(suite->tests)) {
        return -1;
    }
    size_t i = suite->ntest;
    suite->ntest++;
    suite->tests[i] = test;
    return 0;
}


void
capitest_test_run(capitest_Test *test)
{
    printf("%s: ", test->name);
    fflush(stdout);

    test->result = test->func();
    capitest_Result *result = &test->result;

    if (result->result == capitest_enum_FAIL) {
        printf("FAIL at %s:%i", result->filename, result->lineno);
        if (result->msg) {
            printf("(%s)", result->msg);
        }
        printf("\n");
    }
    else {
        assert(result->result == capitest_enum_SUCCESS);
        printf("ok\n");
    }
    fflush(stdout);
}


int
capitest_testsuite_run(capitest_TestSuite *suite)
{
    size_t nfail = 0;
    Py_Initialize();
    for (size_t i=0; i < suite->ntest; i++) {
        capitest_Test *test = &suite->tests[i];
        capitest_test_run(test);
        if (test->result.result != capitest_enum_SUCCESS) {
            nfail++;
        }
    }
    Py_Finalize();
    printf("\n");
    printf("Tests (%i) result: ", suite->ntest);
    if (nfail) {
        printf("%s FAILS\n", nfail);
    }
    else {
        printf("SUCCESS\n");
    }
    if (nfail) {
        return 1;
    }
    else {
        return 0;
    }
}
