OS_NAME := $(shell uname -s | tr A-Z a-z)
MINGW_STRIP = i686-w64-mingw32-strip
MINGW_CC = i686-w64-mingw32-gcc
CFLAGS = -Wall -std=c99 -I.
RELEASE_CFLAGS = -Os -Werror
DEBUG_CFLAGS = -g
#CFLAGS += $(RELEASE_CFLAGS)
CFLAGS += $(DEBUG_CFLAGS)
TEST_SRC = tests.c CuTest.c
TEST_HDR = CuTest.h
PREFIX ?= /usr
SHAREDIR ?= $(PREFIX)/share/games/echeck
EXECDIR ?= $(PREFIX)/games
LOCALEDIR ?= $(PREFIX)/share/locale
default: echeck mofiles
LFLAGS = -lintl
ifeq ($(OS_NAME),darwin)
#	LFLAGS += -Lintl
endif	
all: echeck.zip echeck

mofiles: locale/de/LC_MESSAGES/echeck.mo

install: echeck mofiles
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
	install -t $(LOCALEDIR)/de/LC_MESSAGES locale/de/LC_MESSAGES/echeck.mo

tags:
	@ctags *.c *.h

locale/echeck.pot: echeck.c
	xgettext -d echeck -o $@ -s $^

locale/de/echeck.po: locale/echeck.pot
	msgmerge -o $@ $@ $< 

locale/%/LC_MESSAGES/echeck.mo: locale/%/echeck.po 
	@rm -f $@
	@mkdir -p $@
	@rmdir $@
	msgfmt $< -o $@

echeck.exe: echeck.c unicode.c unicode.h config.h
	$(MINGW_CC) $(CFLAGS) -o echeck.exe echeck.c unicode.c
	$(MINGW_STRIP) echeck.exe

echeck: echeck.c unicode.c unicode.h config.h
	$(CC) $(LFLAGS) $(CFLAGS) -o echeck echeck.c unicode.c

tests: echeck.c unicode.c unicode.h config.h $(TEST_SRC) $(TEST_HDR)
	$(CC) $(CFLAGS) -DTESTING -o tests echeck.c unicode.c $(TEST_SRC)

clean:
	@rm -f *.o core *.bak tests echeck echeck.exe echeck.zip

check: tests
	@./tests -T=all -Lde -Re2 -b

echeck.zip: echeck.exe changelog.txt LIESMICH.txt README.txt
	zip -r echeck.zip echeck.exe e2 e3 changelog.txt LIESMICH.txt README.txt
