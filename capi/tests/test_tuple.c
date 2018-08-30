#include <Python.h>

typedef enum {
    capitest_enum_SUCCESS,
    capitest_enum_FAIL,
    capitest_enum_SKIPPED
} capitest_ResultEnum;

typedef struct {
    capitest_ResultEnum result;
    const char *msg;
    const char *filename;
    int lineno;
} capitest_Result;

typedef capitest_Result (*capitest_TestFunc) ();

typedef struct {
    capitest_Result result;
    const char *name;
    capitest_TestFunc func;
} capitest_Test;

typedef struct {
    size_t ntest;
    /* FIXME: don't use hardcoded array size */
    capitest_Test tests[10];
} capitest_TestSuite;

#define capitest_TestSuite_INIT {.ntest = 0}

#define capitest_FAIL_MSG(message) \
    (capitest_Result){.result = capitest_enum_FAIL, .msg = (message), .filename = __FILE__, .lineno = __LINE__}
#define capitest_FAIL(message) capitest_FAIL_MSG(NULL)
#define capitest_SUCCESS() \
    (capitest_Result){.result = capitest_enum_SUCCESS, .filename = __FILE__, .lineno = __LINE__}

static capitest_Result
check_PyTuple_New_Size(size_t size)
{
    PyObject *tuple = PyTuple_New(size);
    if (tuple == NULL) {
        return capitest_FAIL();
    }
    if (Py_TYPE(tuple) != &PyTuple_Type) {
        return capitest_FAIL();
    }
    /* don't check Py_REFCNT() to PyTuple_New(0): CPython uses a singleton */
    if (size != 0) {
        if (Py_REFCNT(tuple) != 1) {
            return capitest_FAIL();
        }
    }
    if (PyTuple_Size(tuple) != size) {
        return capitest_FAIL();
    }
    Py_DECREF(tuple);
    return capitest_SUCCESS();
}


static capitest_Result
test_PyTuple_New(void)
{
    capitest_Result result;
    result = check_PyTuple_New_Size(0);
    if (result.result != capitest_enum_SUCCESS) {
        return result;
    }
    result = check_PyTuple_New_Size(1);
    if (result.result != capitest_enum_SUCCESS) {
        return result;
    }
    result = check_PyTuple_New_Size(5);
    return result;
}


static capitest_Result
test_PyTuple_GET_ITEM(void)
{
    PyObject *tuple = PyTuple_New(0);
    if (tuple == NULL) {
        return capitest_FAIL();
    }
    Py_DECREF(tuple);
    return capitest_SUCCESS();
}


static void
capitest_run_test(capitest_Test *test)
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

/* FIXME: handle error */
static void
capitest_register(capitest_TestSuite *suite, const char *name, capitest_TestFunc func)
{
    capitest_Test test = {.name = name, .func = func};
    assert(suite->ntest < Py_ARRAY_LENGTH(suite->tests));
    size_t i = suite->ntest;
    suite->ntest++;
    suite->tests[i] = test;
}

#define CAPITEST_REGISTER(suite, test_func) capitest_register(&suite, #test_func, test_func)

static void
capitest_run_testsuite(capitest_TestSuite *suite)
{
    Py_Initialize();
    for (size_t i=0; i < suite->ntest; i++) {
        capitest_Test *test = &suite->tests[i];
        capitest_run_test(test);
        /* FIXME: store result somewhere */
    }
    Py_Finalize();
}

int main()
{
    capitest_TestSuite suite = capitest_TestSuite_INIT ;
    CAPITEST_REGISTER(suite, test_PyTuple_New);
    CAPITEST_REGISTER(suite, test_PyTuple_GET_ITEM);
    capitest_run_testsuite(&suite);
}
