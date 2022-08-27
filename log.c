#include <stdarg.h>
#include <stdio.h>

#include "xtilez.h"

void xt_log(unsigned int priority, const char *format, ...) {
  va_list args;
  va_start(args, format);

  char level_token;
  FILE *output_stream;

  switch (priority) {
  case XT_LOG_LEVEL_DEBUG:
    output_stream = stderr;
    level_token = 'D';
    break;
  case XT_LOG_LEVEL_INFO:
    output_stream = stdout;
    level_token = 'I';
    break;
  case XT_LOG_LEVEL_WARN:
    output_stream = stderr;
    level_token = 'W';
    break;
  case XT_LOG_LEVEL_ERROR:
    output_stream = stderr;
    level_token = 'E';
    break;
  case XT_LOG_LEVEL_FATAL:
    output_stream = stderr;
    level_token = 'F';
    break;
  default:
    fprintf(stderr, "XTilez[F]: Unknown log level type: %d\n", priority);
    exit(1);
  }

  char buffer[BUFSIZ];
  const char *prefix = "XTilez[";

  unsigned int buf_idx = 0;
  // Add the prefix
  for (; buf_idx < BUFSIZ - 2; buf_idx++) {
    if (prefix[buf_idx] == '\0') {
      break;
    }
    buffer[buf_idx] = prefix[buf_idx];
  }
  buffer[buf_idx++] = level_token;
  buffer[buf_idx++] = ']';
  buffer[buf_idx++] = ':';
  buffer[buf_idx++] = ' ';

  unsigned int offset = buf_idx;
  while (buf_idx < BUFSIZ - 2) {
    if (format[buf_idx - offset] == '\0') {
      buffer[buf_idx] = '\n';
      buffer[buf_idx + 1] = '\0';
      break;
    }
    buffer[buf_idx] = format[buf_idx - offset];
    buf_idx++;
  }

  vfprintf(output_stream, buffer, args);

  va_end(args);
}
