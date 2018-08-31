CPYTHON_SRC=..
LIBPYTHON=python3.8dm
PYTHON_CFLAGS=-I $(CPYTHON_SRC) -I $(CPYTHON_SRC)/Include
PYTHON_LDFLAGS=-l $(LIBPYTHON) -L $(CPYTHON_SRC)
CFLAGS=-D Py_NEWCAPI $(pkg-config check --cflags --libs) $(PYTHON_CFLAGS)
LDFLAGS=$(pkg-config check --libs) $(PYTHON_LDFLAGS)

all: runtests
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./runtests

runtests: runtests.c test_tuple.c
	$(CC) -g -o $@ $^ -l check $(CFLAGS) $(LDFLAGS)

clean:
	rm -f runtests
