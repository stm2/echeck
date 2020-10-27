MINGW_STRIP = i686-w64-mingw32-strip
MINGW_CC = i686-w64-mingw32-gcc
CFLAGS = -Wall -std=c99 -Werror -I.
RELEASE_CFLAGS = -Os
DEBUG_CFLAGS = -g
CFLAGS += $(RELEASE_CFLAGS)
TEST_SRC = tests.c CuTest.c
TEST_HDR = CuTest.h
PREFIX ?= /usr
SHAREDIR ?= $(PREFIX)/share/games/echeck
EXECDIR ?= $(PREFIX)/games
default: echeck

all: echeck.zip echeck

install: echeck
	install -d $(EXECDIR)
	install -t $(EXECDIR) echeck
	install -d $(SHAREDIR)/e2/de
	install -d $(SHAREDIR)/e2/en
	install -d $(SHAREDIR)/e3/de
	install -d $(SHAREDIR)/e3/en
	install -t $(SHAREDIR)/e2/de e2/de/*.txt
	install -t $(SHAREDIR)/e2/en e2/en/*.txt
	install -t $(SHAREDIR)/e3/de e3/de/*.txt
	install -t $(SHAREDIR)/e3/en e3/en/*.txt

tags:
	@ctags *.c *.h

echeck.exe: echeck.c unicode.c unicode.h config.h
	$(MINGW_CC) $(CFLAGS) -o echeck.exe echeck.c unicode.c
	$(MINGW_STRIP) echeck.exe

echeck: echeck.c unicode.c unicode.h config.h
	$(CC) $(CFLAGS) -o echeck echeck.c unicode.c

tests: echeck.c unicode.c unicode.h config.h $(TEST_SRC) $(TEST_HDR)
	$(CC) $(CFLAGS) -DTESTING -o tests echeck.c unicode.c $(TEST_SRC)

clean:
	@rm -f *.o core *.bak tests echeck echeck.exe echeck.zip

check: tests
	@./tests -T=all -Lde -Re2 -b

echeck.zip: echeck.exe changelog.txt LIESMICH.txt README.txt
	zip -r echeck.zip echeck.exe e2 e3 changelog.txt LIESMICH.txt README.txt
