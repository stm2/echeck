/* which version of C? */
#if defined(__STDC__)
# define C89
# if defined(__STDC_VERSION__)
#  define C90
#  if (__STDC_VERSION__ >= 199409L)
#   define C94
#  endif
#  if (__STDC_VERSION__ >= 199901L)
#   define C99
#   define HAVE__BOOL
#  endif
# endif
#endif

/* platform-specific defines */
#ifdef WIN32
# define PATH_DELIM ";"
#else
# define PATH_DELIM ":"
#endif

/* define your compiler capabilities */

#ifdef _MSC_VER
# define HAVE__STRDUP
# define HAVE__STRICMP
# define HAVE__SNPRINTF
# define HAVE___MINMAX
#endif

#ifdef __GNUC__
# ifdef __STRICT_ANSI__
#  include <strings.h>
# else
#  define HAVE_STRDUP
#  define HAVE_SNPRINTF
# endif
# define HAVE_STRCASECMP
# define HAVE_STDBOOL_H
# define HAVE_SYS_PARAM_H
#endif

#if defined(MAX) && defined(MIN)
#define HAVE_MINMAX
#endif

/* define some common macros that your compiler may name otherwise */

#ifdef HAVE_STDBOOL_H
# include <stdbool.h>
#else
# ifndef HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
typedef unsigned char _Bool;
#  endif
# endif
# define bool _Bool
# define false 0
# define true 1
# define __bool_true_false_are_defined 1
#endif

#if !defined(HAVE_STRDUP) && defined(HAVE__STRDUP)
# define strdup _strdup
# define HAVE_STRDUP
#endif

#if !defined(HAVE_STRDUP)
# define strdup(s) strcpy(malloc(strlen(s)+1), s)
#endif

#if !defined(HAVE_STRICMP) && defined(HAVE_STRCASECMP)
# define stricmp strcasecmp
# define strnicmp strncasecmp
# define HAVE_STRICMP
#endif

#if !defined(HAVE_STRICMP) && defined(HAVE__STRICMP)
# define stricmp _stricmp
# define strnicmp _strnicmp
# define HAVE_STRICMP
#endif

#if !defined(HAVE_SNPRINTF) && defined(HAVE__SNPRINTF)
# define snprintf _snprintf
# define HAVE_SNPRINTF
#endif

#if !defined(HAVE_MINMAX) && defined(HAVE___MINMAX)
# define MIN(a,b) __min(a, b)
# define MAX(a,b) __max(a, b)
# define HAVE_MINMAX
#endif

#if !defined(HAVE_MINMAX) && defined(HAVE_SYS_PARAM_H)
# include <sys/param.h>
# define HAVE_MINMAX
#endif

#if !defined(HAVE_MINMAX)
# define MIN(a,b) ((a) < (b) ? (a) : (b))
# define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
