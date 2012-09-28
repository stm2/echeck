CC = gcc
CFLAGS = -g -Wall -std=c99 -Werror
# CFLAGS = -O2 -fomit-frame-pointer -Wall
CFLAGS += -Icutest -DWITH_CUTEST

ifeq ($(PREFIX),)
PREFIX=/home/eressea/echeck
endif

all: echeck test

install: echeck
	install -t $(PREFIX) echeck
	install -d $(PREFIX)/e2/de
	install -d $(PREFIX)/e2/en
	install -d $(PREFIX)/e3/de
	install -d $(PREFIX)/e3/en
	install -t $(PREFIX)/e2/de e2/de/*.txt
	install -t $(PREFIX)/e2/en e2/en/*.txt
	install -t $(PREFIX)/e3/de e3/de/*.txt
	install -t $(PREFIX)/e3/en e3/en/*.txt

tags:
	@ctags *.c *.h

echeck: echeck.c unicode.c unicode.h config.h tests.c cutest/CuTest.c cutest/CuTest.h
	$(CC) $(CFLAGS) $(LFLAGS) -o echeck echeck.c unicode.c tests.c cutest/CuTest.c

clean:
	@rm -f *.o core *.bak echeck
	@rm -f e2/test/echeck_output.txt e2/test/echeck_errors.txt e3/test/echeck_output.txt e3/test/echeck_errors.txt

test: echeck
	@./echeck -T=all -Lde -Re2 -b
	./echeck  -Re2 -oe2/test/echeck_output.txt -Oe2/test/echeck_errors.txt e2/test/orders.test.txt > /dev/null
	./echeck  -Re3 -oe3/test/echeck_output.txt -Oe3/test/echeck_errors.txt e3/test/orders.test.txt > /dev/null
	diff -q e2/test/echeck_output.txt e2/test/echeck_output.template
	diff -q e2/test/echeck_errors.txt e2/test/echeck_errors.template
	diff -q e3/test/echeck_output.txt e3/test/echeck_output.template
	diff -q e3/test/echeck_errors.txt e3/test/echeck_errors.template
	@echo "tests completed successfully"

