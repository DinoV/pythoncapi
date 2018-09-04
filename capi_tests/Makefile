CPYTHON_SRC=..
LIBPYTHON=python3.8dm
PYTHON_CFLAGS=-Wall -I $(CPYTHON_SRC) -I $(CPYTHON_SRC)/Include
PYTHON_LDFLAGS=-l $(LIBPYTHON) -L $(CPYTHON_SRC)
CFLAGS=$(shell pkg-config check --cflags) $(PYTHON_CFLAGS)
LDFLAGS=$(shell pkg-config check --libs) $(PYTHON_LDFLAGS)
SOURCES=runtests.c test_object.c test_tuple.c

#ENV_RUNTEST=LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib
# CK_FORK=no to detect reference leaks
ENV_RUNTEST=CK_FORK=no LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib

.PHONY: test_fullapi test_newcapi test_no_borrow test_no_struct
.NOTPARALLEL: test_fullapi test_newcapi test_no_borrow test_no_struct

all: test_newcapi

testmatrix: test_fullapi test_newcapi test_no_borrow test_no_struct

test_fullapi: runtests_fullapi
	@echo "=== Full API tests ==="
	$(ENV_RUNTEST) ./runtests_fullapi
	@echo

test_newcapi: runtests_newcapi
	@echo "=== New C API tests (Py_NEWCAPI) ==="
	$(ENV_RUNTEST) ./runtests_newcapi
	@echo

test_no_borrow: runtests_no_borrow
	@echo "=== No borrowed reference tests (Py_NEWCAPI_BORROWED_REF) ==="
	$(ENV_RUNTEST) ./runtests_no_borrow
	@echo

test_no_struct: runtests_no_struct
	@echo "=== No struct tests (Py_NEWCAPI_NO_STRUCT) ==="
	$(ENV_RUNTEST) ./runtests_no_struct
	@echo

runtests_fullapi: $(SOURCES)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LDFLAGS)

runtests_newcapi: $(SOURCES)
	$(CC) -g -o $@ $^ -D Py_NEWCAPI $(CFLAGS) $(LDFLAGS)

runtests_no_borrow: $(SOURCES)
	$(CC) -g -o $@ $^ -D Py_NEWCAPI_BORROWED_REF $(CFLAGS) $(LDFLAGS)

runtests_no_struct: $(SOURCES)
	$(CC) -g -o $@ $^ -D Py_NEWCAPI_NO_STRUCT $(CFLAGS) $(LDFLAGS)

clean:
	rm -f runtests_fullapi runtests_newcapi runtests_no_borrow
