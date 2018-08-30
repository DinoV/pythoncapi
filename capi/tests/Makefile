CPYTHON_SRC=../..
LIBPYTHON=python3.8dm

all: runtests
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./runtests

runtests: runtests.c capitest.c test_tuple.c
	$(CC) -D Py_NEWCAPI -o $@ $^ -I $(CPYTHON_SRC) -I $(CPYTHON_SRC)/Include -l $(LIBPYTHON) -L $(CPYTHON_SRC)

clean:
	rm runtests
