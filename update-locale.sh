#!/bin/sh
xgettext -k_ -kt -d echeck -o po/echeck.pot -s echeck.c
msgmerge -o po/echeck.de.po po/echeck.de.po po/echeck.pot
mkdir -p locale/de/LC_MESSAGES
msgfmt po/echeck.de.po -o locale/de/LC_MESSAGES/echeck.mo

