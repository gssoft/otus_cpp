#pragma once

#ifndef BASE_LOGGER_HPP
#define BASE_LOGGER_HPP

#include <string>

// ������ �����������
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

// ��������� �������� �������
class BaseLogger {
public:
    virtual ~BaseLogger() = default;

    // ����������� ����� �����������
    virtual void Log(LogLevel level, const std::string& message) = 0;
};

#endif // BASE_LOGGER_HPP

