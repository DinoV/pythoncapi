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


START_TEST(test_Py_REFCNT)
{
    PyObject *obj = Py_None;
    Py_ssize_t refcnt = Py_REFCNT(obj);

    Py_INCREF(obj);
    ck_assert_int_eq(Py_REFCNT(obj), refcnt + 1);
    Py_DECREF(obj);
}
END_TEST


START_TEST(test_Py_TYPE)
{
    PyObject *obj = PyLong_FromLong(5);
    PyTypeObject *expected = &PyLong_Type;
    Py_ssize_t refcnt = Py_REFCNT(expected);

    PyTypeObject *type = Py_TYPE(obj);
    ck_assert_ptr_eq(type, expected);
    ck_assert_int_eq(Py_REFCNT(expected), refcnt);   /* borrowed ref */

    Py_DECREF(obj);
}
END_TEST


START_TEST(test_Py_SIZE)
{
    PyObject *obj = Py_BuildValue("(ii)", 3, 4);

    Py_ssize_t size = Py_SIZE(obj);
    ck_assert_int_eq(size, 2);

    Py_DECREF(obj);
}
END_TEST


void register_PyObject(Suite *s)
{
    TCase *testcase = tcase_create("PyObject");
    tcase_add_test(testcase, test_Py_INCREF);
    tcase_add_test(testcase, test_Py_DECREF);
    tcase_add_test(testcase, test_Py_REFCNT);
    tcase_add_test(testcase, test_Py_TYPE);
    tcase_add_test(testcase, test_Py_SIZE);
    suite_add_tcase(s, testcase);
}
