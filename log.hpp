#ifndef LOG_HPP_INCLUDED
#define LOG_HPP_INCLUDED

#include "platform.hpp"

#define STR_LOG_INFO "INFO"
#define STR_LOG_WARN "WARN"
#define STR_LOG_ERROR "ERROR"

void log(int level, const char* format, ...);

#endif // LOG_HPP_INCLUDED
