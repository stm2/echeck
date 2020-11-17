#!/bin/sh
xgettext -k_ -kt -d echeck -o locale/echeck.pot -s echeck.c
msgmerge -o locale/de/echeck.po locale/de/echeck.po locale/echeck.pot
mkdir -p locale/de/LC_MESSAGES
msgfmt locale/de/echeck.po -o locale/de/LC_MESSAGES/echeck.mo

