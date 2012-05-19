#include <stdarg.h>

#include "log.hpp"

#ifdef __ANDROID__

void log(int level, const char* format, ...)
{
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // send to android logger
  __android_log_vprint(level, APP_NAME, format, arguments);

    // don't forget to clean up!
  va_end(arguments);
}

#else // LINUX, MAC, WINDOWS

void log(int level, const char* format, ...)
{
  // get arguments
  va_list arguments;
  va_start(arguments, format);

  // get the string corresponding to logger level
  switch(level)
  {
    case LOG_INFO:
      printf(STR_LOG_INFO " - ");
    break;

    case LOG_WARN:
      printf(STR_LOG_WARN " - ");
    break;

    case LOG_ERROR:
      printf(STR_LOG_ERROR " - ");
    break;

    default:
    break;
  }

  // send to standard output and flush
  vprintf(format, arguments);
  puts("");

  // don't forget to clean up!
  va_end(arguments);
}

#endif // #ifdef __ANDROID__
