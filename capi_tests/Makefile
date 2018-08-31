CPYTHON_SRC=..
LIBPYTHON=python3.8dm
PYTHON_CFLAGS=-I $(CPYTHON_SRC) -I $(CPYTHON_SRC)/Include
PYTHON_LDFLAGS=-l $(LIBPYTHON) -L $(CPYTHON_SRC)
CFLAGS=$(shell pkg-config check --cflags) $(PYTHON_CFLAGS)
LDFLAGS=$(shell pkg-config check --libs) $(PYTHON_LDFLAGS)
SOURCES=runtests.c test_tuple.c

all: runtests_fullapi runtests_newcapi runtests_no_borrow
	@echo "=== Full API tests ==="
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./runtests_fullapi
	@echo "=== New C API tests ==="
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./runtests_newcapi
	@echo "=== No borrowed reference tests ==="
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./runtests_no_borrow

runtests_fullapi: $(SOURCES)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDFLAGS)

runtests_newcapi: $(SOURCES)
	$(CC) -g -o $@ $^ -D Py_NEWCAPI $(CFLAGS) $(LDFLAGS)

runtests_no_borrow: $(SOURCES)
	$(CC) -g -o $@ $^ -D Py_NEWCAPI_BORROWED_REF $(CFLAGS) $(LDFLAGS)

clean:
	rm -f runtests_fullapi runtests_newcapi runtests_no_borrow
