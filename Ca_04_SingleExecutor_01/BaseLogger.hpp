#pragma once

#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include <string>

// Уровни логирования
enum class LogLevel {
    SUCCESS,
    WARNING,
    FATAL,
    INFO,
    DEBUG,
    TRACE,
    ERROR,
    EXCEPTION,
    CRITICAL
};

// Интерфейс базового логгера
class BaseLogger {
public:
    virtual ~BaseLogger() = default;

    // Виртуальный метод логирования
    virtual void Log(LogLevel level, const std::string& message) = 0;
};

#endif // BASE_LOGGER_HPP

