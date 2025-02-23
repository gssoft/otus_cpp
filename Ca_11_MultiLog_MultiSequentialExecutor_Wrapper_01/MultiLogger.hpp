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

// Перечисление статусов сообщения
enum class MessageStatus { SUCCESS, WARNING, FATAL };

// Структура лог-сообщения
struct LogItem {
    int id;                     // Уникальный ID сообщения
    int channel_id;             // ID канала
    std::string time;           // Время в строковом формате
    MessageStatus status;       // Уровень сообщения
    std::string description;    // Текст сообщения

    // Преобразование в строку
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

// Базовый интерфейс для конкретных логгеров
class ILogger {
protected:
    int channel_id = 0; // ID канала, по умолчанию 0 (общий канал)

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
    std::vector<std::shared_ptr<ILogger>> loggers; // Логгеры
    Executor& executor;                            // Исполнитель задач
    int idCounter = 1;                             // Уникальный идентификатор для логов

public:
    explicit MultiLogger(Executor& exec) : executor(exec) {}

    // Добавление нового логгера
    void addLogger(const std::shared_ptr<ILogger>& logger) {
        // Привязываем логгер к каналу
        // logger->setup_channel_id(static_cast<int>(loggers.size() + 1)); // ID канала начинается с 1
        if (logger->get_channel_id() == 0) {
            // Если `channel_id` не установлен, задаем его автоматически
            logger->setup_channel_id(static_cast<int>(loggers.size() + 1));
        }

        loggers.push_back(logger);
    }

    // Логирование
    void log(MessageStatus status, const std::string& description) {
        auto timePoint = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        char timeBuffer[26];
        ctime_s(timeBuffer, sizeof(timeBuffer), &time);

        std::string timeStr(timeBuffer);
        timeStr.pop_back(); // Убираем символ перевода строки

        // Цикл по всем логгерам
        for (const auto& logger : loggers) {
            if (logger->isEnabled()) {
                // Для каждого логгера создается собственный LogItem с channel_id
                LogItem item{ idCounter++, logger->get_channel_id(), timeStr, status, description }; // !!!!!!!!!!!!!!!!!

                // Передаем задачу на исполнение через executor
                executor.SubmitTask(item.channel_id - 1, [logger, item]() {
                    logger->log(item);
                    });
            }
        }
    }

    // Вывод всех логов
    void printAll() const {
        for (const auto& logger : loggers) {
            logger->print();
        }
    }
};

#endif // MULTILOGGER_HPP

