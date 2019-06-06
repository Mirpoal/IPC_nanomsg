#ifndef NNMSG_LOGGER_H_
#define NNMSG_LOGGER_H_

#ifdef DEBUG
#define ADK_DEBUG_DEFAULT_LEVEL 2
#else
#define ADK_DEBUG_DEFAULT_LEVEL 0
#endif

namespace eshare {
class Logger {
public:
    Logger();
    ~Logger();

	typedef enum {
        kErrorLevel    = 0,
        kWarningLevel  = 1,
        kDebugLevel    = 2,
        kInfoLevel     = 3,
    } DebugLevel;

	static void Error(const char* tag, const char* format, ...);
    static void Warning(const char* tag, const char* format, ...);
    static void Debug(const char* tag, const char* format, ...);
    static void Info(const char* tag, const char* format, ...);

	static DebugLevel debug_level(void) {
        return _debug_level;
    }
	
	static void set_debug_level(const DebugLevel level) {
        _debug_level = level;
    }

private:
    static DebugLevel _debug_level;
};

#ifndef TAG
#define TAG "sys"
#endif


#define pr_err(format, ...) eshare::Logger::Error(TAG, format, ##__VA_ARGS__)
#define pr_dbg(format, ...) eshare::Logger::Debug(TAG, format, ##__VA_ARGS__)
#define pr_warn(format, ...) eshare::Logger::Warning(TAG, format, ##__VA_ARGS__)
#define pr_info(format, ...) eshare::Logger::Info(TAG, format, ##__VA_ARGS__)

#define debug_func_enter() eshare::Logger::Info(__func__, "enter\n")
#define debug_func_exit()  eshare::Logger::Info(__func__, "exit\n")


}


#endif // UTILS_LOGGER_H_

