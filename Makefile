CC = gcc
CFLAGS = -g -Wall
# CFLAGS = -O2 -fomit-frame-pointer -Wall
#LFLAGS = -s
BINDIR = /usr/local/bin
LIBDIR = /usr/local/lib/echeck

all: echeck

install: echeck
	cp echeck $(BINDIR)
	cp de/*.txt $(LIBDIR)/de
	cp en/*.txt $(LIBDIR)/en

tags:
	@ctags *.c

echeck: echeck.c
	$(CC) $(CFLAGS) $(LFLAGS) -o echeck echeck.c

clean:
	@rm -f *.o core *.bak echeck
