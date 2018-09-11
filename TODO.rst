TODO list for new Python C API
==============================

* Write a tool to automate migration from the old C API to the new C API
* capi_tests: check for reference leaks
* Modify PyObject_INIT(op, type) macro to return op again? use an inlined
  function?
* Do we need ``PySequence_Fast_GetItemRef()``?
* Enhance documentation to document reference counting

Replace Py_TYPE()
=================o

Dealloc::

    Py_TYPE(self)->tp_free((PyObject *)self);

becomes::

    PyTypeObject *type = Py_GetType(self);
    type->tp_free((PyObject *)self);
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

Borrowed Reference APIs
=======================

These APIs all return borrowed references and need to be updated to not do so::

    PyObject* PyObject_Init(PyObject *op, PyTypeObject *type)
    PyObject* PyFunction_GetCode(PyObject *op)

    PyVarObject* PyObject_InitVar(PyVarObject *op, PyTypeObject *type, Py_ssize_t size)

    PyObject* PyEval_GetBuiltins()
    PyObject* PyEval_GetLocals()
    PyObject* PyEval_GetGlobals()
    PyFrameObject* PyEval_GetFrame()

    PyObject* PyFunction_GetCode(PyObject *op)
    PyObject* PyFunction_GetGlobals(PyObject *op)
    PyObject* PyFunction_GetModule(PyObject *op)
    PyObject* PyFunction_GetDefaults(PyObject *op)
    PyObject* PyFunction_GetClosure(PyObject *op)

    PyObject* PyMethod_Function(PyObject *meth)
    PyObject* PyMethod_GET_FUNCTION(PyObject *meth)
    PyObject* PyMethod_Self(PyObject *meth)
    PyObject* PyMethod_GET_SELF(PyObject *meth)¶

    PyObject* PyDict_GetItem(PyObject *p, PyObject *key)
    PyObject* PyDict_GetItemWithError(PyObject *p, PyObject *key)
    PyObject* PyDict_GetItemString(PyObject *p, const char *key)
    PyObject* PyDict_SetDefault(PyObject *p, PyObject *key, PyObject *default)
    int PyDict_Next(PyObject *p, Py_ssize_t *ppos, PyObject **pkey, PyObject **pvalue)

    PyObject* PyList_GetItem(PyObject *list, Py_ssize_t index)

    PyObject* PyList_GET_ITEM(PyObject *list, Py_ssize_t i)

    PyObject* PyCell_GET(PyObject *cell)

    PyObject* PySequence_Fast_GET_ITEM(PyObject *o, Py_ssize_t i)

    PyObject *PySys_GetObject(const char *name)
    PyObject *PySys_GetXOptions()

    PyObject* PyImport_AddModule(const char *name)
    PyObject* PyImport_GetModuleDict()

    PyObject* PyErr_Occurred()
    PyObject* PyThreadState_GetDict()