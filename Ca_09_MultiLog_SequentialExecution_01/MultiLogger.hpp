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

// Перечисление статусов сообщения
enum class MessageStatus { SUCCESS, WARNING, FATAL };

// Структура лог-сообщения
struct LogItem {
    int id;                     // ID канала
    std::string time;           // Время в строковом формате
    MessageStatus status;       // Уровень сообщения
    std::string description;    // Текст сообщения

    // Преобразование в строку
    std::string toString() const {
        std::ostringstream oss;
        oss << "[" << id << "] ["
            << (status == MessageStatus::SUCCESS ? "SUCCESS" :
                status == MessageStatus::WARNING ? "WARNING" : "FATAL") << "] "
            << description;
        return oss.str();
    }
};

// Базовый интерфейс для конкретных логгеров
class ILogger {
protected:
    int channel_id = 0;  // ID канала, по умолчанию 0 (общий канал)

public:
    virtual ~ILogger() = default;

    // Указывает, активен ли логгер
    virtual void enable(bool status) = 0;

    // Логирование конкретного сообщения
    virtual void log(const LogItem& item) = 0;

    // Вывод всех логов (если поддерживается)
    virtual void print() const = 0;

    // Проверка: включен ли логгер
    virtual bool isEnabled() const = 0;

    // Установка формата времени
    virtual void set_dt_format(DateTimeFormat format) = 0;

    // Установка ID канала
    void setup_channel_id(int id) {
        channel_id = id;
    }

    // Получение ID канала
    int get_channel_id() const {
        return channel_id;
    }
};

// Класс-менеджер MultiLogger
template <typename Executor>
class MultiLogger {
private:
    std::vector<std::shared_ptr<ILogger>> loggers;
    Executor& executor; // Ссылка на Executor (SingleExecutor) // ******************
    int idCounter = 1; // Уникальный идентификатор для логов

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
        timeStr.pop_back(); // Убираем символ перевода строки

        LogItem item{ idCounter++, timeStr, status, description };

        for (const auto& logger : loggers) {
            if (logger->isEnabled()) {
                // Каждый логгер автоматом добавляет свой channel_id
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

