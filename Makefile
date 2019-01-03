MINGW_STRIP = i686-w64-mingw32-strip
MINGW_CC = i686-w64-mingw32-gcc
CFLAGS = -Wall -std=c99 -Werror -I.
RELEASE_CFLAGS = $(CFLAGS) -Os
DEBUG_CFLAGS = $(CFLAGS) -g -DWITH_CUTEST
TEST_SRC = tests.c CuTest.c CuTest.h
ifeq ($(PREFIX),)
PREFIX=/home/eressea/echeck
endif

all: echeck.exe echeck

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

echeck.exe: echeck.c unicode.c unicode.h config.h
	$(MINGW_CC) $(RELEASE_CFLAGS) -o echeck.exe echeck.c unicode.c
	$(MINGW_STRIP) echeck.exe

echeck: echeck.c unicode.c unicode.h config.h $(TEST_SRC)
	$(CC) $(DEBUG_CFLAGS) -DWITH_CUTEST -o echeck echeck.c unicode.c $(TEST_SRC)

clean:
	@rm -f *.o core *.bak echeck echeck.exe

test: echeck
	@./echeck -T=all -Lde -Re2 -b

