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

#include <windows.h>
#include "echeck.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
  _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
  return echeck_main(__argc, __argv);
}
