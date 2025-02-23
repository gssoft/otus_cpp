#pragma once

#ifndef MULTILOGGER_HPP
#define MULTILOGGER_HPP

#include "DateTimeFormatter.hpp"
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>
#include <functional>

// ������������ �������� ���������
enum class MessageStatus { SUCCESS, WARNING, FATAL };

// ��������� ���-���������
struct LogItem {
    int id;                     // ���������� ID ���������
    int channel_id;             // ID ������
    std::string time;           // ����� � ��������� �������
    MessageStatus status;       // ������� ���������
    std::string description;    // ����� ���������

    // �������������� � ������
    std::string toString() const {
        std::ostringstream oss;
        // oss << "[" << id << "] [" << channel_id << "] ["
        oss << "[" << channel_id << "] ["
            << (status == MessageStatus::SUCCESS ? "SUCCESS" :
                status == MessageStatus::WARNING ? "WARNING" : "FATAL") << "] "
            << description;
        return oss.str();
    }
};

// ������� ��������� ��� ���������� ��������
class ILogger {
protected:
    int channel_id = 0; // ID ������, �� ��������� 0 (����� �����)

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
    std::vector<std::shared_ptr<ILogger>> loggers; // �������
    Executor& executor;                            // ����������� �����
    int idCounter = 1;                             // ���������� ������������� ��� �����

public:
    explicit MultiLogger(Executor& exec) : executor(exec) {}

    // ���������� ������ �������
    void addLogger(const std::shared_ptr<ILogger>& logger) {
        // ����������� ������ � ������
        // logger->setup_channel_id(static_cast<int>(loggers.size() + 1)); // ID ������ ���������� � 1
        if (logger->get_channel_id() == 0) {
            // ���� `channel_id` �� ����������, ������ ��� �������������
            logger->setup_channel_id(static_cast<int>(loggers.size() + 1));
        }

        loggers.push_back(logger);
    }

    // �����������
    void log(MessageStatus status, const std::string& description) {
        auto timePoint = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        char timeBuffer[26];
        ctime_s(timeBuffer, sizeof(timeBuffer), &time);

        std::string timeStr(timeBuffer);
        timeStr.pop_back(); // ������� ������ �������� ������

        // ���� �� ���� ��������
        for (const auto& logger : loggers) {
            if (logger->isEnabled()) {
                // ��� ������� ������� ��������� ����������� LogItem � channel_id
                LogItem item{ idCounter++, logger->get_channel_id(), timeStr, status, description }; // !!!!!!!!!!!!!!!!!

                // �������� ������ �� ���������� ����� executor
                executor.SubmitTask(item.channel_id - 1, [logger, item]() {
                    logger->log(item);
                    });
            }
        }
    }

    // ����� ���� �����
    void printAll() const {
        for (const auto& logger : loggers) {
            logger->print();
        }
    }
};

#endif // MULTILOGGER_HPP

