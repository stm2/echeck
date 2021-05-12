OSNAME := $(shell uname -s | tr A-Z a-z)

ifeq ($(OSNAME),darwin)
LFLAGS += -lintl
PREFIX ?= /usr/local
EXECDIR ?= $(PREFIX)/bin
SHAREDIR ?= $(PREFIX)/share/echeck
endif	

MINGW_STRIP = i686-w64-mingw32-strip
MINGW_CC = i686-w64-mingw32-gcc
CFLAGS = -Wall -std=c99 -I. -DHAVE_GETTEXT -D_DEFAULT_SOURCE
RELEASE_CFLAGS = -Os -Werror
DEBUG_CFLAGS = -g
TEST_SRC = tests.c CuTest.c
TEST_HDR = CuTest.h
PREFIX ?= /usr
SHAREDIR ?= $(PREFIX)/share/games/echeck
EXECDIR ?= $(PREFIX)/games
LOCALEDIR ?= $(PREFIX)/share/locale

default: echeck mofiles

all: echeck mofiles

pofiles: po/echeck.de.po 

mofiles: pofiles locale/de/LC_MESSAGES/echeck.mo

install: echeck mofiles
	install -d $(EXECDIR)
	install -C echeck $(EXECDIR) 
	install -d $(SHAREDIR)/e2/de
	install -d $(SHAREDIR)/e2/en
	install -d $(SHAREDIR)/e3/de
	install -d $(SHAREDIR)/e3/en
	install -d $(SHAREDIR)/locale/de/LC_MESSAGES
	install -C -m 644 e2/de/*.txt $(SHAREDIR)/e2/de 
	install -C -m 644 e2/en/*.txt $(SHAREDIR)/e2/en 
	install -C -m 644 e3/de/*.txt $(SHAREDIR)/e3/de 
	install -C -m 644 e3/en/*.txt $(SHAREDIR)/e3/en 
	install -C -m 644 locale/de/LC_MESSAGES/*.mo $(SHAREDIR)/locale/de/LC_MESSAGES

tags:
	@ctags *.c *.h

po/echeck.pot: echeck.c
	xgettext --no-location -k_ -kt -d echeck -o $@ -s $^

po/echeck.%.po: po/echeck.pot
	msgmerge -N -U $@ $< 

locale/%/LC_MESSAGES/echeck.mo: po/echeck.de.po 
	@rm -rf $@
	@mkdir -p $@
	@rmdir $@
	msgfmt -f -c $< -o $@

echeck: main.c echeck.c unicode.c unicode.h whereami.c whereami.h
	$(CC) $(LFLAGS) $(CFLAGS) $(RELEASE_CFLAGS) -o echeck main.c echeck.c unicode.c whereami.c

tests: echeck.c whereami.c whereami.h unicode.c unicode.h $(TEST_SRC) $(TEST_HDR)
	$(CC) $(LFLAGS) $(CFLAGS) $(DEBUG_FLAGS) -DTESTING -o tests echeck.c whereami.c unicode.c $(TEST_SRC)

clean:
	@rm -f *.o core *.bak tests echeck echeck.exe echeck.zip 

check: tests
	@./tests -T=all -Lde -Re2 -b
