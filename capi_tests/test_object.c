/* Test PyObject API */

#include <stdlib.h>
#include <check.h>
#include <Python.h>


#define TEST_INCREF(INCREF_FUNC) \
    do { \
        PyObject *obj = Py_None; \
        Py_ssize_t refcnt = Py_REFCNT(obj); \
        \
        INCREF_FUNC(obj); \
        ck_assert_int_eq(Py_REFCNT(obj), refcnt + 1); \
        \
        Py_DECREF(obj); \
    } while (0)

START_TEST(test_Py_INCREF)
{
    TEST_INCREF(Py_INCREF);
}
END_TEST


START_TEST(test_Py_XINCREF)
{
    TEST_INCREF(Py_XINCREF);

    /* Py_XINCREF(NULL) does nothing: it must not crash */
    Py_XINCREF(NULL);
}
END_TEST


START_TEST(test_Py_IncRef)
{
    TEST_INCREF(Py_IncRef);

    /* Py_IncRef(NULL) does nothing: it must not crash */
    Py_IncRef(NULL);
}


#define TEST_DECREF(DECREF_FUNC) \
    do { \
        PyObject *obj = Py_None; \
        Py_INCREF(obj); \
        Py_ssize_t refcnt = Py_REFCNT(obj); \
        \
        DECREF_FUNC(obj); \
        ck_assert_int_eq(Py_REFCNT(obj), refcnt - 1); \
    } while (0)

END_TEST
START_TEST(test_Py_DECREF)
{
    TEST_DECREF(Py_DECREF);
}
END_TEST


START_TEST(test_Py_XDECREF)
{
    TEST_DECREF(Py_XDECREF);

    /* Py_XDECREF(NULL) does nothing: it must not crash */
    Py_XDECREF(NULL);
}
END_TEST


START_TEST(test_Py_DecRef)
{
    TEST_DECREF(Py_DecRef);

    /* Py_DecRef(NULL) does nothing: it must not crash */
    Py_DecRef(NULL);
}
END_TEST


START_TEST(test_Py_CLEAR)
{
    PyObject *obj = Py_None;
    Py_INCREF(obj);
    Py_ssize_t refcnt = Py_REFCNT(Py_None);

    Py_CLEAR(obj);
    ck_assert_int_eq(Py_REFCNT(Py_None), refcnt - 1);
    ck_assert_ptr_null(obj);

    obj = NULL;
    /* Py_CLEAR(NULL) does nothing: it must not crash */
    Py_CLEAR(obj);
}
END_TEST


#define TEST_SETSET(SETREF_FUNC) \
    do { \
        PyObject *value1 = Py_True; \
        PyObject *value2 = Py_False; \
        \
        Py_INCREF(value1); \
        PyObject *obj = value1; \
        Py_ssize_t refcnt1 = Py_REFCNT(value1); \
        Py_ssize_t refcnt2 = Py_REFCNT(value2); \
        \
        SETREF_FUNC(obj, value2); \
        ck_assert_ptr_eq(obj, value2); \
        ck_assert_int_eq(Py_REFCNT(value1), refcnt1 - 1); \
        ck_assert_int_eq(Py_REFCNT(value2), refcnt2); \
    } while (0)

START_TEST(test_Py_SETREF)
{
    TEST_SETSET(Py_SETREF);
}
END_TEST


START_TEST(test_Py_XSETREF)
{
    TEST_SETSET(Py_XSETREF);

    /* Py_XSETREF(NULL, value) must not crash */
    PyObject *value3 = Py_None;
    Py_ssize_t refcnt3 = Py_REFCNT(value3);
    PyObject *obj = NULL;
    Py_XSETREF(obj, value3);
    ck_assert_ptr_eq(obj, value3);
    ck_assert_int_eq(Py_REFCNT(value3), refcnt3);
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


#ifndef Py_NEWCAPI
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
#endif


START_TEST(test_Py_SIZE)
{
    PyObject *obj = Py_BuildValue("(ii)", 3, 4);

    Py_ssize_t size = Py_SIZE(obj);
    ck_assert_int_eq(size, 2);

    Py_DECREF(obj);
}
END_TEST


static PyObject*
return_none(void)
{
    Py_RETURN_NONE;
}

#define TEST_RETURN_FUNC(VALUE, RETURN_FUNC) \
    do { \
        PyObject *value = (VALUE); \
        Py_ssize_t refcnt = Py_REFCNT(value); \
        PyObject *obj = RETURN_FUNC(); \
        \
        ck_assert_ptr_eq(obj, value); \
        ck_assert_int_eq(Py_REFCNT(value), refcnt + 1); \
        \
        Py_DECREF(obj); \
    } while (0)

START_TEST(test_Py_RETURN_NONE)
{
    TEST_RETURN_FUNC(Py_None, return_none);
}
END_TEST


static PyObject*
return_true(void)
{
    Py_RETURN_TRUE;
}

START_TEST(test_Py_RETURN_TRUE)
{
    TEST_RETURN_FUNC(Py_True, return_true);
}
END_TEST


static PyObject*
return_false(void)
{
    Py_RETURN_FALSE;
}

START_TEST(test_Py_RETURN_FALSE)
{
    TEST_RETURN_FUNC(Py_False, return_false);
}
END_TEST


static PyObject*
return_not_implemented(void)
{
    Py_RETURN_NOTIMPLEMENTED;
}

START_TEST(test_Py_RETURN_NOTIMPLEMENTED)
{
    TEST_RETURN_FUNC(Py_NotImplemented, return_not_implemented);
}
END_TEST


static PyObject*
return_rich_compare(int a, int b, int op)
{
    Py_RETURN_RICHCOMPARE(a, b, op);
}


START_TEST(test_Py_RETURN_RICHCOMPARE)
{
#define TEST_CMP(A, OP, B, VALUE) \
    do { \
        PyObject *value = (VALUE); \
        Py_ssize_t refcnt = Py_REFCNT(value); \
        PyObject *obj = return_rich_compare((A), (B), (OP)); \
        ck_assert_ptr_eq(obj, value); \
        ck_assert_int_eq(Py_REFCNT(value), refcnt + 1); \
        Py_DECREF(obj); \
    } while (0)

    /* 1 == 1 */
    TEST_CMP(1, Py_EQ, 1, Py_True);
    TEST_CMP(1, Py_NE, 1, Py_False);
    TEST_CMP(1, Py_LT, 1, Py_False);
    TEST_CMP(1, Py_GT, 1, Py_False);
    TEST_CMP(1, Py_LE, 1, Py_True);
    TEST_CMP(1, Py_GE, 1, Py_True);

    /* 1 == 2 */
    TEST_CMP(1, Py_EQ, 2, Py_False);
    TEST_CMP(1, Py_NE, 2, Py_True);
    TEST_CMP(1, Py_LT, 2, Py_True);
    TEST_CMP(1, Py_GT, 2, Py_False);
    TEST_CMP(1, Py_LE, 2, Py_True);
    TEST_CMP(1, Py_GE, 2, Py_False);
}
END_TEST


void register_PyObject(Suite *s)
{
    TCase *testcase = tcase_create("PyObject");
    tcase_add_test(testcase, test_Py_INCREF);
    tcase_add_test(testcase, test_Py_XINCREF);
    tcase_add_test(testcase, test_Py_IncRef);

    tcase_add_test(testcase, test_Py_DECREF);
    tcase_add_test(testcase, test_Py_XDECREF);
    tcase_add_test(testcase, test_Py_DecRef);

    tcase_add_test(testcase, test_Py_CLEAR);
    tcase_add_test(testcase, test_Py_SETREF);
    tcase_add_test(testcase, test_Py_XSETREF);
    tcase_add_test(testcase, test_Py_REFCNT);

#ifndef Py_NEWCAPI
    tcase_add_test(testcase, test_Py_TYPE);
#endif
    tcase_add_test(testcase, test_Py_SIZE);

    tcase_add_test(testcase, test_Py_RETURN_NONE);
    tcase_add_test(testcase, test_Py_RETURN_TRUE);
    tcase_add_test(testcase, test_Py_RETURN_FALSE);
    tcase_add_test(testcase, test_Py_RETURN_NOTIMPLEMENTED);
    tcase_add_test(testcase, test_Py_RETURN_RICHCOMPARE);
    suite_add_tcase(s, testcase);
}
