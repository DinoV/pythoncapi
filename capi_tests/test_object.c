/* Test PyObject API */

#include <stdlib.h>
#include <check.h>
#include <Python.h>


START_TEST(test_Py_INCREF)
{
    PyObject *obj = Py_None;
    Py_ssize_t refcnt = Py_REFCNT(obj);

    Py_INCREF(obj);
    ck_assert_int_eq(Py_REFCNT(obj), refcnt + 1);

    Py_DECREF(obj);
}
END_TEST


START_TEST(test_Py_DECREF)
{
    PyObject *obj = Py_None;
    Py_INCREF(obj);
    Py_ssize_t refcnt = Py_REFCNT(obj);

    Py_DECREF(obj);
    ck_assert_int_eq(Py_REFCNT(obj), refcnt - 1);
}
END_TEST


void register_PyObject(Suite *s)
{
    TCase *testcase = tcase_create("PyObject");
    tcase_add_test(testcase, test_Py_INCREF);
    tcase_add_test(testcase, test_Py_DECREF);
    suite_add_tcase(s, testcase);
}
