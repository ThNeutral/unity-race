#include <logger/Logger.h>

#include <iostream>

constexpr const char* RED = "\033[1;31m";
constexpr const char* GREEN = "\033[1;32m";
constexpr const char* YELLOW = "\033[1;33m";
constexpr const char* RESET = "\033[0m";


void Logger::ReportInfo(const char* funcName, const char* message) {
    std::cout << GREEN
              << funcName << ":  " << message
              << RESET << std::endl;
}

void Logger::ReportWarning(const char* funcName, const char* message) {
    std::cout << YELLOW
              << funcName << ":  " << message
              << RESET << std::endl;
}

void Logger::ReportError(const char* funcName, const char* message) {
    std::cerr << RED
              << funcName << " failed: " << message
              << RESET << std::endl;
}

void Logger::ReportError(const char* funcName, int code, const char* message) {
    std::cerr << RED
              << funcName << " failed with code " << code << ": " << message
              << RESET << std::endl;
}

void Logger::ReportFatal(const char* funcName, const char* message) {
    ReportFatal(funcName, message, std::exception());
}

void Logger::ReportFatal(const char* funcName, const char* message, std::exception exception) {
    std::cerr << RED
              << funcName << " encountered a fatal error: " << message
              << RESET << std::endl;

    throw exception;
}

void Logger::ReportFatal(const char* funcName, int code, const char* message) {
    ReportFatal(funcName, code, message, std::exception());
}

void Logger::ReportFatal(const char* funcName, int code, const char* message, std::exception exception) {
    std::cerr << RED
              << funcName << " encountered a fatal error with code " << code << ": " << message
              << RESET << std::endl;

    throw exception;
}