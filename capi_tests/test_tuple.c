/* Test PyTuple C API */

/* FIXME: write tests for:
   - PyTuple_Type
   - PyTupleIter_Type
   - PyTuple_Check()
   - PyTuple_SetItem()
   - PyTuple_SET_ITEM()
   - PyTuple_GetSlice()
*/

/* Not tested in CPython 3.8 full API:
   - PyTupleObject
   - _PyTuple_Resize()
   - _PyTuple_MaybeUntrack()
   - PyTuple_ClearFreeList()
   - _PyTuple_DebugMallocStats()
*/

#include <stdlib.h>
#include <check.h>
#include <Python.h>

static void
check_PyTuple_New(size_t size)
{
    PyObject *tuple = PyTuple_New(size);
    ck_assert_ptr_nonnull(tuple);
    ck_assert(PyTuple_CheckExact(tuple));
    /* don't check Py_REFCNT() on PyTuple_New(0): CPython uses a singleton */
    if (size != 0) {
        ck_assert_int_eq(Py_REFCNT(tuple), 1);
    }
    ck_assert_int_eq(PyTuple_Size(tuple), size);
    Py_DECREF(tuple);
}


START_TEST(test_PyTuple_New)
{
    check_PyTuple_New(0);
    check_PyTuple_New(1);
    check_PyTuple_New(5);

    PyObject *tuple = PyTuple_New(-1);
    ck_assert_ptr_null(tuple);
}
END_TEST


#ifndef Py_NO_BORROWED_REF
static void
check_PyTuple_GetItem(int use_macro)
{
    const size_t size = 3;
    PyObject* items[size];   /* strong references */
    PyObject *tuple = PyTuple_New(size);
    ck_assert_ptr_nonnull(tuple);
    for (int i=i; i < size; i++) {
        PyObject *obj = PyLong_FromLong(i);
        ck_assert_ptr_nonnull(obj);
        int res = PyTuple_SetItem(tuple, i, obj);
        ck_assert_int_eq(res, 0);
        items[i] = obj;
    }

    for (int i=i; i < size; i++) {
        PyObject *obj;   /* borrowed reference */
        Py_ssize_t refcnt = Py_REFCNT(items[i]);
        if (use_macro) {
            obj = PyTuple_GET_ITEM(tuple, i);
        }
        else {
            obj = PyTuple_GetItem(tuple, i);
        }
        ck_assert_ptr_eq(obj, items[i]);
        ck_assert_int_eq(Py_REFCNT(obj), refcnt);
    }

    for (int i=i; i < size; i++) {
        Py_DECREF(items[i]);
    }
    Py_DECREF(tuple);
}


START_TEST(test_PyTuple_GetItem)
{
    check_PyTuple_GetItem(0);
}
END_TEST


START_TEST(test_PyTuple_GET_ITEM)
{
    check_PyTuple_GetItem(1);
}
END_TEST
#endif /* Py_NO_BORROWED_REF */


static void
check_PyTuple_SizeN(size_t size, int use_macro)
{
    PyObject* items[size];   /* borrowed references */
    PyObject *tuple = PyTuple_New(size);
    ck_assert_ptr_nonnull(tuple);
    for (int i=i; i < size; i++) {
        int res = PyTuple_SetItem(tuple, i, Py_None);
        ck_assert_int_eq(res, 0);
    }
    Py_ssize_t tuple_size;
    if (use_macro) {
        tuple_size = PyTuple_GET_SIZE(tuple);
    }
    else {
        tuple_size = PyTuple_Size(tuple);
    }
    ck_assert_int_eq(tuple_size, size);
    Py_DECREF(tuple);
}


static void
check_PyTuple_Size(int use_macro)
{
    check_PyTuple_SizeN(0, use_macro);
    check_PyTuple_SizeN(1, use_macro);
    check_PyTuple_SizeN(2, use_macro);
    check_PyTuple_SizeN(5, use_macro);
}


START_TEST(test_PyTuple_Size)
{
    check_PyTuple_Size(0);
}
END_TEST


START_TEST(test_PyTuple_GET_SIZE)
{
    check_PyTuple_Size(1);
}
END_TEST


START_TEST(test_PyTuple_CheckExact)
{
    PyObject *tuple = PyTuple_New(0);
    ck_assert_ptr_nonnull(tuple);
    ck_assert(PyTuple_CheckExact(tuple));
    Py_DECREF(tuple);

    PyObject *list = PyList_New(0);
    ck_assert_ptr_nonnull(list);
    ck_assert(!PyTuple_CheckExact(list));
    Py_DECREF(list);

}
END_TEST


START_TEST(test_PyTuple_Pack)
{
    const size_t size = 3;
    PyObject* items[size];

    for (int i=0; i < size; i++) {
        items[i] = PyLong_FromLong(i);
        ck_assert_ptr_nonnull(items[i]);
    }

    PyObject *tuple = PyTuple_Pack(size, items[0], items[1], items[2]);
    ck_assert_int_eq(PyTuple_Size(tuple), size);
    for (int i=0; i < size; i++) {
        PyObject *item = PyTuple_GetItemRef(tuple, i);
        ck_assert_ptr_eq(item, items[i]);
        Py_DECREF(item);
    }

    for (int i=0; i < size; i++) {
        Py_DECREF(items[i]);
    }
}
END_TEST


void register_PyTuple(Suite *s)
{
    TCase *tc_core = tcase_create("Py_Tuple");
    tcase_add_test(tc_core, test_PyTuple_New);
#ifndef Py_NO_BORROWED_REF
    tcase_add_test(tc_core, test_PyTuple_GetItem);
    tcase_add_test(tc_core, test_PyTuple_GET_ITEM);
#endif
    tcase_add_test(tc_core, test_PyTuple_Size);
    tcase_add_test(tc_core, test_PyTuple_GET_SIZE);
    tcase_add_test(tc_core, test_PyTuple_CheckExact);
    tcase_add_test(tc_core, test_PyTuple_Pack);
    suite_add_tcase(s, tc_core);
}
