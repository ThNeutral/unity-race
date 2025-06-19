#ifndef LOGGER_H
#define LOGGER_H

#include <exception>

class Logger {
    public:
        static void ReportInfo(const char* funcName, const char* message);
        
        static void ReportWarning(const char* funcName, const char* message);
        
        static void ReportError(const char* funcName, const char* message);
        static void ReportError(const char* funcName, int code, const char* message);

        static void ReportFatal(const char* funcName, const char* message);
        static void ReportFatal(const char* funcName, const char* message, std::exception exception);

        static void ReportFatal(const char* funcName, int code, const char* message);
        static void ReportFatal(const char* funcName, int code, const char* message, std::exception exception);
};

#endif