TODO list for new Python C API
==============================

* capi_tests: check for reference leaks
* Modify PyObject_INIT(op, type) macro to return op again? use an inlined
  function?
* Do we need ``PySequence_Fast_GetItemRef()``?

Issues
======

Following code becomes invalid::

        (void)PyObject_INIT(im, &PyMethod_Type);
        Py_SIZE(obj) = size;   /* must use _Py_SET_SIZE(obj, size); */
        Py_TYPE(obj) = type;   /* must use _Py_SET_TYPE(obj, type); */
