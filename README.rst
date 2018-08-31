Design a new better C API for Python
====================================

This project is a fork of the development version of the future CPython 3.8.
The intent is to experiment to implement the new C API described at:

   https://pythoncapi.readthedocs.io/

Install dependencies on Fedora::

   dnf install -y make gcc check

Build and run unit tests::

   ./configure --enable-shared --with-pydebug
   make
   cd capi_tests
   make  # only test Py_NEWCAPI
   # or to run the full test matrix: make testmatrix

The changes live in the **pythoncapi** branch. See also:

* `github.com/pythoncapi/cpython <https://github.com/pythoncapi/cpython>`_
* `capi-sig mailing list
  <https://mail.python.org/mm3/mailman3/lists/capi-sig.python.org/>`_
