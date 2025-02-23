#pragma once

#ifndef MULTILOGGER_HPP
#define MULTILOGGER_HPP

#include "DateTimeFormatter.hpp"
#include "SingleExecutor.hpp"
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <time.h>
#include <functional>

// ������������ �������� ���������
enum class MessageStatus { SUCCESS, WARNING, FATAL };

// ��������� ���-���������
struct LogItem {
    int id;                     // ID ������
    std::string time;           // ����� � ��������� �������
    MessageStatus status;       // ������� ���������
    std::string description;    // ����� ���������

    // �������������� � ������
    std::string toString() const {
        std::ostringstream oss;
        oss << "[" << id << "] ["
            << (status == MessageStatus::SUCCESS ? "SUCCESS" :
                status == MessageStatus::WARNING ? "WARNING" : "FATAL") << "] "
            << description;
        return oss.str();
    }
};

// ������� ��������� ��� ���������� ��������
class ILogger {
protected:
    int channel_id = 0;  // ID ������, �� ��������� 0 (����� �����)

public:
    virtual ~ILogger() = default;

    // ���������, ������� �� ������
    virtual void enable(bool status) = 0;

    // ����������� ����������� ���������
    virtual void log(const LogItem& item) = 0;

    // ����� ���� ����� (���� ��������������)
    virtual void print() const = 0;

    // ��������: ������� �� ������
    virtual bool isEnabled() const = 0;

    // ��������� ������� �������
    virtual void set_dt_format(DateTimeFormat format) = 0;

    // ��������� ID ������
    void setup_channel_id(int id) {
        channel_id = id;
    }

    // ��������� ID ������
    int get_channel_id() const {
        return channel_id;
    }
};

// �����-�������� MultiLogger
template <typename Executor>
class MultiLogger {
private:
    std::vector<std::shared_ptr<ILogger>> loggers;
    Executor& executor; // ������ �� Executor (SingleExecutor) // ******************
    int idCounter = 1; // ���������� ������������� ��� �����

public:

    explicit MultiLogger(Executor& exec) : executor(exec) {} // ******************

    void addLogger(const std::shared_ptr<ILogger>& logger) {
        loggers.push_back(logger);
    }

    void log(MessageStatus status, const std::string& description) {
        auto timePoint = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

        char timeBuffer[26];
        ctime_s(timeBuffer, sizeof(timeBuffer), &time);

        std::string timeStr(timeBuffer);
        timeStr.pop_back(); // ������� ������ �������� ������

        LogItem item{ idCounter++, timeStr, status, description };

        for (const auto& logger : loggers) {
            if (logger->isEnabled()) {
                // ������ ������ ��������� ��������� ���� channel_id
                item.id = logger->get_channel_id();
                logger->log(item);
            }
        }
    }

    void printAll() const {
        for (const auto& logger : loggers) {
            logger->print();
        }
    }
};

#endif // MULTILOGGER_HPP

