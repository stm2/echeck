/* platform-specific defines */
#ifdef WIN32
# define PATH_DELIM ";"
#else
# define PATH_DELIM ":"
#endif

/* define your compiler capabilities */

#if defined(_MSC_VER)
# define HAVE__STRDUP
# define HAVE__STRICMP
# define HAVE__SNPRINTF
#elif defined(__GNUC__)
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
