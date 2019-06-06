#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

namespace eshare {

#define LOGGER_MAX_LINE_SIZE 1024

// initial logger level
Logger::DebugLevel Logger::_debug_level = Logger::DebugLevel(ADK_DEBUG_DEFAULT_LEVEL);

void Logger::Error(const char* tag, const char* format, ...) {
	char line[LOGGER_MAX_LINE_SIZE] = { 0 };
	va_list v;

	va_start(v, format);
	vsnprintf(line, sizeof(line), format, v);
	va_end(v);

	if (_debug_level >= kErrorLevel)
		printf("%s: %s", tag, line);
}

void Logger::Warning(const char* tag, const char* format, ...) {
	char line[LOGGER_MAX_LINE_SIZE] = { 0 };
	va_list v;

	va_start(v, format);
	vsnprintf(line, sizeof(line), format, v);
	va_end(v);

	if (_debug_level >= kWarningLevel)
		printf("%s: %s", tag, line);
}

void Logger::Debug(const char* tag, const char* format, ...) {
	char line[LOGGER_MAX_LINE_SIZE] = { 0 };
	va_list v;

	va_start(v, format);
	vsnprintf(line, sizeof(line), format, v);
	va_end(v);

	if (_debug_level >= kDebugLevel)
		printf("%s: %s", tag, line);
}

void Logger::Info(const char* tag, const char* format, ...) {
	char line[LOGGER_MAX_LINE_SIZE] = { 0 };
	va_list v;

	va_start(v, format);
	vsnprintf(line, sizeof(line), format, v);
	va_end(v);

	if (_debug_level >= kInfoLevel)
		printf("%s: %s", tag, line);
}

}  // end of namespace
