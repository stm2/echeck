CC = gcc
CFLAGS = -g -Wall -ansi
# CFLAGS = -O2 -fomit-frame-pointer -Wall
ifeq ($(PREFIX),)
PREFIX=/home/eressea/echeck
endif

all: echeck

install: echeck
	install -t $(PREFIX) echeck
	install -d $(PREFIX)/common/de
	install -d $(PREFIX)/common/en
	install -d $(PREFIX)/e2/de
	install -d $(PREFIX)/e2/en
	install -d $(PREFIX)/e3/de
	install -d $(PREFIX)/e3/en
	install -t $(PREFIX)/common/de common/de/*.txt
	install -t $(PREFIX)/common/en common/en/*.txt
	install -t $(PREFIX)/e2/de e2/de/*.txt
	install -t $(PREFIX)/e2/en e2/en/*.txt
	install -t $(PREFIX)/e3/de e3/de/*.txt
	install -t $(PREFIX)/e3/en e3/en/*.txt

test: echeck
	./echeck  -Re2 -oe2/test/echeck_output.txt -Oe2/test/echeck_errors.txt e2/test/orders.test.txt > /dev/null
	./echeck  -Re3 -oe3/test/echeck_output.txt -Oe3/test/echeck_errors.txt e3/test/orders.test.txt > /dev/null
	diff -q e2/test/echeck_output.txt e2/test/echeck_output.template
	diff -q e2/test/echeck_errors.txt e2/test/echeck_errors.template
	diff -q e3/test/echeck_output.txt e3/test/echeck_output.template
	diff -q e3/test/echeck_errors.txt e3/test/echeck_errors.template
	@echo "tests completed successfully"

tags:
	@ctags *.c *.h

echeck: echeck.c unicode.c unicode.h config.h
	$(CC) $(CFLAGS) $(LFLAGS) -o echeck echeck.c unicode.c

clean:
	@rm -f *.o core *.bak echeck
	@rm -f e2/test/echeck_output.txt e2/test/echeck_errors.txt e3/test/echeck_output.txt e3/test/echeck_errors.txt
