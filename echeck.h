/*
 * Eressea PBeM Order Syntax Checker Copyright 1997-2021
 * - Enno Rehling (enno@eressea.de)
 * - Christian Schlittchen (corwin@amber.kn-bremen.de)
 * - Katja Zedel (katze@felidae.kn-bremen.de)
 * - Henning Peters (faroul@faroul.de)
 * based on:
 * - Atlantis v1.0 Copyright 1993 by Russell Wallace
 * - Atlantis v1.7 Copyright 1996 by Alex Schröder
 * This program is freeware. It may not be sold or used commercially.
 * Please send any changes or bugfixes to the authors.
 */

#pragma once


struct unit;

extern int error_count;
extern int warning_count;
extern int verbose;
extern int brief;
extern int show_warnings;
extern int line_no;

void echeck_init(void);
int readfiles(void);
int echeck_main(int argc, char *argv[]);

int get_long_order_line(void);
void mock_input(const char *input);
char *getbuf(void);
char *igetstr(char *);
void checkgiving(void);
void checkanorder(char *);
struct unit *newunit(int n, int t);
void set_order_unit(struct unit *u);
void process_order_file(int *faction_count, int *unit_count);
