#pragma once

#include "Native_stdafx.h"
#include <string>

// Default intitialize, must be define before use NativeLogger class
// Can be change base usage
#define _DEFAULT_LOG_ENABLED_				true
#define _DEFAULT_LOG_LEVEL_ENABLED_			Core::Helper::NativeLogger::Level::LOG_HDBG
#define _DEFAULT_LOG_2_FILE_ENABLED_		true
#define _DEFAULT_LOG_FILE_DIR_				"E:\\gaLogger.txt"
#define _DEFAULT_LOG_COLLECTION_ENABLED_	false	//Use for android
#define _DEFAULT_LOG_COLLECTION_TAG_		"ATHU"	//Use for android

#define _LOG_MUTEX_ENABLED_					1

namespace Core {
	namespace Helper {
		class NativeLogger{
		public:
			enum Level
			{
				LOG_NONE,
				LOG_ERROR,
				LOG_WARN,
				LOG_INFO,
				LOG_DBG,
				LOG_HDBG,
				LOG_SDBG,
			};
		private:
			static bool _logEnabled;
			static bool _log2FileEnabled;
			static bool _logCollectionEnabled;
			static Level _logLevelActived;
			static std::string _logFileName;
			static std::string _logCollectionTag;
		private:
			static void print(Level logLevel, const char *tag, const char *format, va_list args);
			static void toFile(const char *message);
			static void fillLog(char *logConatain, const char *type, const char *tag, char *msg);
		public:
			static void initialize(bool logEnabled, Level logLevelActived, bool log2FileEnabled, const char *logFileName, bool logCollectionEnabled, const char *logCollectionTag);
			static void error(const char *tag, const char *format, ...);
			static void info(const char *tag, const char *fromat, ...);
			static void warning(const char *tag, const char *fromat, ...);
			static void debug(const char *tag, const char *fromat, ...);
			static void hdebug(const char *tag, const char *fromat, ...);
			static void sdebug(const char *tag, const char *fromat, ...);
		};
	}
}


#ifdef _DEBUG
#	define INFO_LOG_FMT "[%s:%d] "
#	define ATH_INF(tag,fmt,...) Core::Helper::NativeLogger::info(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#	define ATH_ERR(tag,fmt,...) Core::Helper::NativeLogger::error(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#	define ATH_WRN(tag,fmt,...) Core::Helper::NativeLogger::warning(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#	define ATH_DBG(tag,fmt,...) Core::Helper::NativeLogger::debug(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#	define ATH_HDBG(tag,fmt,...) Core::Helper::NativeLogger::hdebug(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#	define ATH_SDBG(tag,fmt,...) Core::Helper::NativeLogger::sdebug(tag,INFO_LOG_FMT fmt,__FILE__,__LINE__,__VA_ARGS__)
#else 
#	define ATH_INF(tag,fmt,...) Core::Helper::NativeLogger::info(tag,fmt,__VA_ARGS__)
#	define ATH_ERR(tag,fmt,...) Core::Helper::NativeLogger::error(tag,fmt,__VA_ARGS__)
#	define ATH_WRN(tag,fmt,...) Core::Helper::NativeLogger::warning(tag,fmt,__VA_ARGS__)
#	define ATH_DBG(tag,fmt,...) Core::Helper::NativeLogger::debug(tag,fmt,__VA_ARGS__)
#	define ATH_HDBG(tag,fmt,...) Core::Helper::NativeLogger::hdebug(tag,fmt,__VA_ARGS__)
#	define ATH_SDBG(tag,fmt,...) Core::Helper::NativeLogger::sdebug(tag,fmt,__VA_ARGS__)
#endif