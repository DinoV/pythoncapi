CPYTHON_SRC=../..
LIBPYTHON=python3.8dm

all: test_tuple
	LD_LIBRARY_PATH=$(CPYTHON_SRC) PYTHONPATH=$(CPYTHON_SRC)/Lib ./test_tuple

test_tuple: test_tuple.c
	$(CC) -o $@ $< -I $(CPYTHON_SRC) -I $(CPYTHON_SRC)/Include -l $(LIBPYTHON) -L $(CPYTHON_SRC)

