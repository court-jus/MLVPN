#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdarg.h>

#include "tool.h"

int mystr_eq(const char *s1, const char *s2)
{
  if ((s1 == NULL) || (s2 == NULL)) 
    return 0;

  if (strcmp(s1, s2) == 0)
    return 1;

  return 0;
}

void stripBadChar(const char *from, char *to)
{
  if (from == NULL)
    return;

  while (*from != '\0')
  {
    switch (*from)
    {
      case '|':
        *to++ = 'l';
        break;
      case '`':
        *to++ = '\'';
        break;
      case '/':
      case '\\':
        break;

      default:
        if (isascii(*from))
          *to++ = *from;
        break;
    }
    from++;
  }

  *to = '\0';
}

char *
tool_get_bytes(unsigned long long bytes)
{
    char *str;
    char *conv_unit = NULL;
    int conv_div = 1;
    
    if (bytes < 1024)
    {
        conv_unit = "B";
        conv_div = 1;
    } else if (bytes < 1024*1024) {
        conv_unit = "KiB";
        conv_div = 1024;
    } else if (bytes < 1024*1024*1024) {
        conv_unit = "MiB";
        conv_div = 1024*1024;
    } else {
        conv_unit = "GiB";
        conv_div = 1024*1024*1024;
    }

    str = (char *)calloc(32, 1); /* 32 chars should be enough !!! */
    snprintf(str, 32, "%0.3f %s", (double)(bytes / (double)conv_div), conv_unit);
    return str;
}

char *myprintf(const char *format, ...)
{
    char *z_format;
    va_list ap;
    int len, try;
  
    len = strlen(format) + 512; /* Try */
    z_format = malloc(len);
    if (z_format == NULL)
    {
        perror("malloc");
        exit(1);
    }
  
    while ( 1 )
    {
        va_start(ap, format);
        try = vsnprintf (z_format, len, format, ap);
        va_end(ap);
        if (try > -1 && try < len)
            break;
        if (try > -1)
            len = try+1;
        else
            len *= 2; /* Ugly =) */
        z_format = realloc(z_format, len);

        if (z_format == NULL)
        {
            perror("realloc");
            exit(1);
        }
    }
  
    return z_format;
}
