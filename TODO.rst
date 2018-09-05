TODO list for new Python C API
==============================

* capi_tests: check for reference leaks
* Modify PyObject_INIT(op, type) macro to return op again? use an inlined
  function?
* Do we need ``PySequence_Fast_GetItemRef()``?

Replace Py_TYPE()
=================o

Dealloc::

    Py_TYPE(op)->tp_free((PyObject *)op);

becomes::

    PyTypeObject *type = Py_GetType(op);
    type->tp_free((PyObject *)op);
    Py_DECREF(type);

Size::

    res = _PyObject_SIZE(Py_TYPE(self)) + self->allocated * self->ob_descr->itemsize;

becomes::

    PyTypeObject *type = Py_GetType(self);
    res = _PyObject_SIZE(type) + self->allocated * self->ob_descr->itemsize;
    Py_DECREF(type);

Error::

     PyErr_Format(PyExc_TypeError,
         "first argument must be a type object, not %.200s",
         Py_TYPE(arraytype)->tp_name);

becomes::

     PyErr_Format(PyExc_TypeError,
                  "first argument must be a type object, not %T",
                  arraytype);

Py_BuildValue::

     result = Py_BuildValue(
         "N(CO)O", Py_GetType(self), typecode, list, dict);

becomes::

     result = Py_BuildValue(
         "N(CO)O", Py_GetType(self), typecode, list, dict);


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
