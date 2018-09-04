TODO list for new Python C API
==============================

* capi_tests: check for reference leaks
* Modify PyObject_INIT(op, type) macro to return op again? use an inlined
  function?
* Do we need ``PySequence_Fast_GetItemRef()``?

Remove Py_TYPE()?
=================

Error::

        PyErr_Format(PyExc_TypeError,
                     "Struct() argument 1 must be a str or bytes object, "
                     "not %.200s",
                     Py_TYPE(format)->tp_name);

Dealloc::

   static void
   s_dealloc(PyStructObject *s)
   {
       ...
       Py_TYPE(s)->tp_free((PyObject *)s);
   }

Object size::

    Py_ssize_t size = _PyObject_SIZE(Py_TYPE(self)) + sizeof(formatcode);

Check type::

   #define PyStruct_CheckExact(op) (Py_TYPE(op) == &PyStructType)

Issues
======

Following code becomes invalid::

        (void)PyObject_INIT(im, &PyMethod_Type);
        Py_SIZE(obj) = size;   /* must use _Py_SET_SIZE(obj, size); */
        Py_TYPE(obj) = type;   /* must use _Py_SET_TYPE(obj, type); */
