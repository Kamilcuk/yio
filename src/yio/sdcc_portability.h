#ifndef YYIO_YIO_SDCC_PORTABILITY_H_
#define YYIO_YIO_SDCC_PORTABILITY_H_

#ifdef __SDCC
#define YYIO_REENTRANT __reentrant
#define YYIO_HAS_SYS_TIME_H 0
#else
#define YYIO_REENTRANT
#define YYIO_HAS_SYS_TIME_H 1
#endif

/* If the environment does not provide FILE, we provide stubs to allow compilation. */
#if YYIO_HAS_FILE == 0
#include <errno.h>
#ifndef ENOSYS
#define ENOSYS 38
#endif
#ifndef FILE
typedef struct YYIO_SDCC_FILE_s { int YYIO_SDCC_FILE_unused; } FILE;
#define FILE FILE
#endif
#ifndef stderr
#define stderr ((FILE*)0)
#endif
#ifndef stdout
#define stdout ((FILE*)0)
#endif
#ifndef fprintf
#define fprintf(stream, ...) ((void)(stream), errno = ENOSYS, -1)
#endif
#ifndef fwrite
#define fwrite(ptr, size, nmemb, stream)  \
  ((void)(ptr), (void)(size), (void)(nmemb), (void)(stream), errno = ENOSYS, (size_t)-1)
#endif
#endif

#endif // YYIO_YIO_SDCC_PORTABILITY_H_
