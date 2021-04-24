/* vi: set ts=2:
 * +-------------------+  Christian Schlittchen <corwin@amber.kn-bremen.de>
 * |                   |  Enno Rehling <enno@eressea.de>
 * | Eressea PBEM host |  Katja Zedel <katze@felidae.kn-bremen.de>
 * | (c) 1998 - 2007   |
 * |                   |  This program may not be used, modified or distributed
 * +-------------------+  without prior permission by the authors of Eressea.
 *
 */

#include "unicode.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <wctype.h>

#define B00000000 0x00
#define B10000000 0x80
#define B11000000 0xC0
#define B11100000 0xE0
#define B11110000 0xF0
#define B11111000 0xF8
#define B11111100 0xFC
#define B11111110 0xFE

#define B00111111 0x3F
#define B00011111 0x1F
#define B00001111 0x0F
#define B00000111 0x07
#define B00000011 0x03
#define B00000001 0x01

int unicode_utf8_tolower(utf8_t *op, size_t outlen, const utf8_t *ip) {
  while (*ip) {
    ucs4_t ucs = *ip;
    ucs4_t low;
    size_t size = 1;

    if (ucs & 0x80) {
      int ret = unicode_utf8_to_ucs4(&ucs, ip, &size);
      if (ret != 0) {
        return ret;
      }
    }
    if (size > outlen) {
      return ENOMEM;
    }
    low = towlower((wint_t)ucs);
    if (low == ucs) {
      memcpy(op, ip, size);
      ip += size;
      op += size;
      outlen -= size;
    } else {
      ip += size;
      unicode_ucs4_to_utf8(op, &size, low);
      op += size;
      outlen -= size;
    }
  }

  if (outlen <= 0) {
    return ENOMEM;
  }
  *op = 0;
  return 0;
}

int unicode_latin1_to_utf8(utf8_t *dst, size_t *outlen, const char *in,
                           size_t *inlen) {
  int is = (int)*inlen;
  int os = (int)*outlen;
  const char *ip = in;
  unsigned char *out = (unsigned char *)dst, *op = out;

  while (ip - in < is) {
    unsigned char c = *ip;
    if (c > 0xBF) {
      if (op - out >= os - 1)
        break;
      *op++ = 0xC3;
      *op++ = c - 64;
    } else if (c > 0x7F) {
      if (op - out >= os - 1)
        break;
      *op++ = 0xC2;
      *op++ = c;
    } else {
      if (op - out >= os)
        break;
      *op++ = c;
    }
    ++ip;
  }
  *outlen = op - out;
  *inlen = ip - in;
  return (int)*outlen;
}

int unicode_utf8_strcasecmp(const utf8_t *a, const utf8_t *b) {
  while (*a && *b) {
    int ret;
    size_t size;
    ucs4_t ucsa = *a, ucsb = *b;

    if (ucsa & 0x80) {
      ret = unicode_utf8_to_ucs4(&ucsa, a, &size);
      if (ret != 0)
        return -1;
      a += size;
    } else
      ++a;
    if (ucsb & 0x80) {
      ret = unicode_utf8_to_ucs4(&ucsb, b, &size);
      if (ret != 0)
        return -1;
      b += size;
    } else
      ++b;

    if (ucsb != ucsa) {
      ucsb = towlower((wint_t)ucsb);
      ucsa = towlower((wint_t)ucsa);
      if (ucsb < ucsa)
        return 1;
      if (ucsb > ucsa)
        return -1;
    }
  }
  if (*b)
    return -1;
  if (*a)
    return 1;
  return 0;
}

int unicode_utf8_strncasecmp(const utf8_t *a, const utf8_t *b, size_t len) {
  size_t parsed = 0;
  while (*a && *b) {
    int ret;
    size_t size;
    ucs4_t ucsa = *a, ucsb = *b;

    if (ucsa & 0x80) {
      ret = unicode_utf8_to_ucs4(&ucsa, a, &size);
      if (ret != 0)
        return -1;
      a += size;
      parsed += size;
    } else {
      ++a;
      ++parsed;
    }
    if (ucsb & 0x80) {
      ret = unicode_utf8_to_ucs4(&ucsb, b, &size);
      if (ret != 0)
        return -1;
      b += size;
    } else {
      ++b;
    }
    if (ucsb != ucsa) {
      ucsb = towlower((wint_t)ucsb);
      ucsa = towlower((wint_t)ucsa);
      if (ucsb < ucsa)
        return 1;
      if (ucsb > ucsa)
        return -1;
    }
    if (parsed >= len) {
      return 0;
    }
  }
  if (*b)
    return -1;
  if (*a)
    return 1;
  return 0;
}

/* Convert a UCS-4 character to UTF-8. */
int unicode_ucs4_to_utf8(utf8_t *utf8_character, size_t *size,
                         ucs4_t ucs4_character) {
  int utf8_bytes;

  if (ucs4_character <= 0x0000007F) {
    /* 0xxxxxxx */
    utf8_bytes = 1;
    utf8_character[0] = (char)ucs4_character;
  } else if (ucs4_character <= 0x000007FF) {
    /* 110xxxxx 10xxxxxx */
    utf8_bytes = 2;
    utf8_character[0] = (char)((ucs4_character >> 6) | B11000000);
    utf8_character[1] = (char)((ucs4_character & B00111111) | B10000000);
  } else if (ucs4_character <= 0x0000FFFF) {
    /* 1110xxxx 10xxxxxx 10xxxxxx */
    utf8_bytes = 3;
    utf8_character[0] = (char)((ucs4_character >> 12) | B11100000);
    utf8_character[1] = (char)(((ucs4_character >> 6) & B00111111) | B10000000);
    utf8_character[2] = (char)((ucs4_character & B00111111) | B10000000);
  } else if (ucs4_character <= 0x001FFFFF) {
    /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
    utf8_bytes = 4;
    utf8_character[0] = (char)((ucs4_character >> 18) | B11110000);
    utf8_character[1] =
        (char)(((ucs4_character >> 12) & B00111111) | B10000000);
    utf8_character[2] = (char)(((ucs4_character >> 6) & B00111111) | B10000000);
    utf8_character[3] = (char)((ucs4_character & B00111111) | B10000000);
  } else if (ucs4_character <= 0x03FFFFFF) {
    /* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
    utf8_bytes = 5;
    utf8_character[0] = (char)((ucs4_character >> 24) | B11111000);
    utf8_character[1] =
        (char)(((ucs4_character >> 18) & B00111111) | B10000000);
    utf8_character[2] =
        (char)(((ucs4_character >> 12) & B00111111) | B10000000);
    utf8_character[3] = (char)(((ucs4_character >> 6) & B00111111) | B10000000);
    utf8_character[4] = (char)((ucs4_character & B00111111) | B10000000);
  } else if (ucs4_character <= 0x7FFFFFFF) {
    /* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
    utf8_bytes = 6;
    utf8_character[0] = (char)((ucs4_character >> 30) | B11111100);
    utf8_character[1] =
        (char)(((ucs4_character >> 24) & B00111111) | B10000000);
    utf8_character[2] =
        (char)(((ucs4_character >> 18) & B00111111) | B10000000);
    utf8_character[3] =
        (char)(((ucs4_character >> 12) & B00111111) | B10000000);
    utf8_character[4] = (char)(((ucs4_character >> 6) & B00111111) | B10000000);
    utf8_character[5] = (char)((ucs4_character & B00111111) | B10000000);
  } else {
    return EILSEQ;
  }

  *size = utf8_bytes;

  return 0;
}

/* Convert a UTF-8 encoded character to UCS-4. */
int unicode_utf8_to_ucs4(ucs4_t *ucs4_character, const utf8_t *utf8_string,
                         size_t *length) {
  utf8_t utf8_character = utf8_string[0];

  /* Is the character in the ASCII range? If so, just copy it to the
     output. */
  if (~utf8_character & 0x80) {
    *ucs4_character = utf8_character;
    *length = 1;
  } else if ((utf8_character & 0xE0) == 0xC0) {
    /* A two-byte UTF-8 sequence. Make sure the other byte is good. */
    if (utf8_string[1] != '\0' && (utf8_string[1] & 0xC0) != 0x80) {
      return EILSEQ;
    }

    *ucs4_character =
        ((utf8_string[1] & 0x3F) << 0) + ((utf8_character & 0x1F) << 6);
    *length = 2;
  } else if ((utf8_character & 0xF0) == 0xE0) {
    /* A three-byte UTF-8 sequence. Make sure the other bytes are
       good. */
    if ((utf8_string[1] != '\0') && (utf8_string[1] & 0xC0) != 0x80 &&
        (utf8_string[2] != '\0') && (utf8_string[2] & 0xC0) != 0x80) {
      return EILSEQ;
    }

    *ucs4_character = ((utf8_string[2] & 0x3F) << 0) +
                      ((utf8_string[1] & 0x3F) << 6) +
                      ((utf8_character & 0x0F) << 12);
    *length = 3;
  } else if ((utf8_character & 0xF8) == 0xF0) {
    /* A four-byte UTF-8 sequence. Make sure the other bytes are
       good. */
    if ((utf8_string[1] != '\0') && (utf8_string[1] & 0xC0) != 0x80 &&
        (utf8_string[2] != '\0') && (utf8_string[2] & 0xC0) != 0x80 &&
        (utf8_string[3] != '\0') && (utf8_string[3] & 0xC0) != 0x80) {
      return EILSEQ;
    }

    *ucs4_character =
        ((utf8_string[3] & 0x3F) << 0) + ((utf8_string[2] & 0x3F) << 6) +
        ((utf8_string[1] & 0x3F) << 12) + ((utf8_character & 0x07) << 18);
    *length = 4;
  } else if ((utf8_character & 0xFC) == 0xF8) {
    /* A five-byte UTF-8 sequence. Make sure the other bytes are
       good. */
    if ((utf8_string[1] != '\0') && (utf8_string[1] & 0xC0) != 0x80 &&
        (utf8_string[2] != '\0') && (utf8_string[2] & 0xC0) != 0x80 &&
        (utf8_string[3] != '\0') && (utf8_string[3] & 0xC0) != 0x80 &&
        (utf8_string[4] != '\0') && (utf8_string[4] & 0xC0) != 0x80) {
      return EILSEQ;
    }

    *ucs4_character =
        ((utf8_string[4] & 0x3F) << 0) + ((utf8_string[3] & 0x3F) << 6) +
        ((utf8_string[2] & 0x3F) << 12) + ((utf8_string[1] & 0x3F) << 18) +
        ((utf8_character & 0x03) << 24);
    *length = 5;
  } else if ((utf8_character & 0xFE) == 0xFC) {
    /* A six-byte UTF-8 sequence. Make sure the other bytes are
       good. */
    if ((utf8_string[1] != '\0') && (utf8_string[1] & 0xC0) != 0x80 &&
        (utf8_string[2] != '\0') && (utf8_string[2] & 0xC0) != 0x80 &&
        (utf8_string[3] != '\0') && (utf8_string[3] & 0xC0) != 0x80 &&
        (utf8_string[4] != '\0') && (utf8_string[4] & 0xC0) != 0x80 &&
        (utf8_string[5] != '\0') && (utf8_string[5] & 0xC0) != 0x80) {
      return EILSEQ;
    }

    *ucs4_character =
        ((utf8_string[5] & 0x3F) << 0) + ((utf8_string[4] & 0x3F) << 6) +
        ((utf8_string[3] & 0x3F) << 12) + ((utf8_string[2] & 0x3F) << 18) +
        ((utf8_string[1] & 0x3F) << 24) + ((utf8_character & 0x01) << 30);
    *length = 6;
  } else {
    return EILSEQ;
  }

  return 0;
}
