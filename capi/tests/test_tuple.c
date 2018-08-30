#define Py_NEWCAPI
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
check_PyTuple_GetItem(size_t size, int use_macro)
{
    capitest_Result res;
    PyObject* items[size];   /* strong references */
    PyObject *tuple = PyTuple_New(size);
    if (tuple == NULL) {
        return capitest_FAIL();
    }
    for (int i=i; i < size; i++) {
        PyObject *obj = PyLong_FromLong(i);
        if (obj == NULL) {
            res = capitest_FAIL();
            goto done;
        }
        if (PyTuple_SetItem(tuple, i, obj) < 0) {
            res = capitest_FAIL();
            goto done;
        }
        items[i] = obj;
    }
    for (int i=i; i < size; i++) {
        PyObject *obj;   /* borrowed reference */
        if (use_macro) {
            obj = PyTuple_GET_ITEM(tuple, i);
        }
        else {
            obj = PyTuple_GetItem(tuple, i);
        }
        if (obj != items[i]) {
            res = capitest_FAIL();
            goto done;
        }
    }
    res = capitest_SUCCESS();
done:
    for (int i=i; i < size; i++) {
        Py_DECREF(items[i]);
    }
    Py_DECREF(tuple);
    return res;
}


static capitest_Result
test_PyTuple_GetItem(void)
{
    check_PyTuple_GetItem(3, 0);
}


static capitest_Result
test_PyTuple_GET_ITEM(void)
{
    check_PyTuple_GetItem(3, 1);
}


static capitest_Result
check_PyTuple_SizeN(size_t size, int use_macro)
{
    capitest_Result res = capitest_SUCCESS();
    PyObject* items[size];   /* borrowed references */
    PyObject *tuple = PyTuple_New(size);
    if (tuple == NULL) {
        return capitest_FAIL();
    }
    for (int i=i; i < size; i++) {
        if (PyTuple_SetItem(tuple, i, Py_None) < 0) {
            Py_DECREF(tuple);
            return capitest_FAIL();
        }
    }
    Py_ssize_t tuple_size;
    if (use_macro) {
        tuple_size = PyTuple_GET_SIZE(tuple);
    }
    else {
        tuple_size = PyTuple_Size(tuple);
    }
    if (tuple_size != size) {
        res = capitest_FAIL();
        goto done;
    }
done:
    Py_DECREF(tuple);
    return res;
}


static capitest_Result
check_PyTuple_Size(int use_macro)
{
    check_PyTuple_SizeN(0, use_macro);
    check_PyTuple_SizeN(1, use_macro);
    check_PyTuple_SizeN(2, use_macro);
    check_PyTuple_SizeN(5, use_macro);
}


static capitest_Result
test_PyTuple_Size(void)
{
    check_PyTuple_Size(0);
}


static capitest_Result
test_PyTuple_GET_SIZE(void)
{
    check_PyTuple_Size(1);
}


int
register_test_tuple(capitest_TestSuite *suite)
{
    if (CAPITEST_REGISTER(suite, test_PyTuple_New) < 0) {
        return -1;
    }
    if (CAPITEST_REGISTER(suite, test_PyTuple_GetItem) < 0) {
        return -1;
    }
    if (CAPITEST_REGISTER(suite, test_PyTuple_GET_ITEM) < 0) {
        return -1;
    }
    if (CAPITEST_REGISTER(suite, test_PyTuple_Size) < 0) {
        return -1;
    }
    if (CAPITEST_REGISTER(suite, test_PyTuple_GET_SIZE) < 0) {
        return -1;
    }
    return 0;
}
