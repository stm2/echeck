/* platform-specific defines */
#ifdef WIN32
#define PATH_DELIM ";"
#endif

#ifdef __unix__
#define PATH_DELIM ":"
#endif

/* define your compiler capabilities */

#ifdef _MSC_VER
# define HAVE__STRDUP
# define HAVE__STRICMP
# define HAVE__SNPRINTF
#endif

#ifdef __GNUC__
# define HAVE_STRDUP
# define HAVE_STRCASECMP
# define HAVE_SNPRINTF
#endif

/* define some common macros that your compiler may name otherwise */

#ifdef HAVE__STRDUP
# ifndef HAVE_STRDUP
#  define strdup _strdup
#  define HAVE_STRDUP
# endif
#endif

#ifdef HAVE_STRCASECMP
# ifndef HAVE_STRICMP
#  define stricmp strcasecmp
#  define strnicmp strncasecmp
#  define HAVE_STRICMP
# endif
#endif

#ifdef HAVE__STRICMP
# ifndef HAVE_STRICMP
#  define stricmp _stricmp
#  define strnicmp _strnicmp
#  define HAVE_STRICMP
# endif
#endif

#ifdef HAVE__SNPRINTF
# ifndef HAVE_SNPRINTF
#  define snprintf _snprintf
#  define HAVE_SNPRINTF
# endif
#endif
