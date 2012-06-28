CC = gcc
CFLAGS = -g -Wall -ansi
# CFLAGS = -O2 -fomit-frame-pointer -Wall
ifeq ($(PREFIX),)
PREFIX=/home/eressea/echeck
endif

all: echeck

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

echeck: echeck.c unicode.c unicode.h config.h
	$(CC) $(CFLAGS) $(LFLAGS) -o echeck echeck.c unicode.c

clean:
	@rm -f *.o core *.bak echeck
