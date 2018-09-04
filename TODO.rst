TODO list for new Python C API
==============================

* capi_tests: check for reference leaks

Issues
======

Following code becomes invalid::

        (void)PyObject_INIT(im, &PyMethod_Type);
