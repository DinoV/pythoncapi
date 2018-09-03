#include <stdio.h>
#include <check.h>
#include <Python.h>

void register_PyObject(Suite *s);
void register_PyTuple(Suite *s);

int main(void)
{
#if defined(PY_VERSION_HEX) && PY_VERSION_HEX >= 0x03080000
    _PyCoreConfig config = _PyCoreConfig_INIT;
    config._frozen = 1;   /* Disable warnings about missing prefix */
    _PyInitError err = _Py_InitializeFromConfig(&config);
    if (_Py_INIT_FAILED(err)) {
        _Py_FatalInitError(err);
    }
#else
    Py_Initialize();
#endif

    Suite *s = suite_create("CAPI");
    register_PyObject(s);
    register_PyTuple(s);
    SRunner *sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    Py_Finalize();

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
