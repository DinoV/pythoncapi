#include <Python.h>
#include "capitest.h"

static capitest_Result
check_PyTuple_New_Size(size_t size)
{
    PyObject *tuple = PyTuple_New(size);
    if (tuple == NULL) {
        return capitest_FAIL();
    }
    if (!PyTuple_CheckExact(tuple)) {
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

#define CAPITEST_REGISTER(suite, test_func) capitest_register(&suite, #test_func, test_func)


int main()
{
    capitest_TestSuite suite = capitest_TestSuite_INIT ;
    CAPITEST_REGISTER(suite, test_PyTuple_New);
    CAPITEST_REGISTER(suite, test_PyTuple_GET_ITEM);
    return capitest_run_testsuite(&suite);
}
