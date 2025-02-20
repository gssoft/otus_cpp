#pragma once

#ifndef CONSOLE_LOGGER_HPP
#define CONSOLE_LOGGER_HPP

#include "BaseLogger.hpp"
#include <iostream>
#include <mutex>
#include <string>

class ConsoleLogger : public BaseLogger {
public:
    ConsoleLogger();  // �����������
    ~ConsoleLogger() override = default;

    // ���������� ������ �����������
    void Log(LogLevel level, const std::string& message) override;

private:
    std::mutex mutex_;  // ��� ����������������� ������
    std::string FormatLogLevel(LogLevel level);
};

#endif // CONSOLE_LOGGER_HPP

