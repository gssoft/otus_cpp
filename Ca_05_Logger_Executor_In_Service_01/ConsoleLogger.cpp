#include "ConsoleLogger.hpp"
#include <iomanip>
#include <chrono>
#include <sstream>
#include <iostream>

ConsoleLogger::ConsoleLogger() {}

std::string ConsoleLogger::FormatLogLevel(LogLevel level) {
    switch (level) {
    case LogLevel::SUCCESS: return "[SUCCESS] ";
    case LogLevel::WARNING: return "[WARNING] ";
    case LogLevel::FATAL:   return "[FATAL] ";
    case LogLevel::INFO:    return "[INFO] ";
    case LogLevel::DEBUG:   return "[DEBUG] ";
    case LogLevel::TRACE:   return "[TRACE] ";
    case LogLevel::ERROR:   return "[ERROR] ";
    case LogLevel::EXCEPTION: return "[EXCEPTION] ";
    case LogLevel::CRITICAL: return "[CRITICAL] ";
    }
    return "[UNKNOWN] ";
}

void ConsoleLogger::Log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);  // Потокобезопасность

    // Форматируем время
    auto now = std::chrono::system_clock::now();
    auto currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm{};
    localtime_s(&local_tm, &currentTime);

    std::ostringstream timeStream;
    timeStream << std::put_time(&local_tm, "%H:%M:%S");

    // Выводим в консоль лог-сообщение
    std::cout << "[" << timeStream.str() << "] " << FormatLogLevel(level) << message << std::endl;
}
