#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "DateTimeFormatter.hpp"
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <time.h>

// Перечисление статусов сообщения
enum class MessageStatus { SUCCESS, WARNING, FATAL };

// Структура лог-сообщения
struct LogItem {
    int id;
    std::string time;            // Время в строковом формате
    MessageStatus status;        // Уровень сообщения
    std::string description;     // Текст сообщения

    // Преобразование в строку
    std::string toString() const {
        std::ostringstream oss;
        /*oss << "[" << id << "] [" << time << "] ["
            << (status == MessageStatus::SUCCESS ? "SUCCESS" :
                status == MessageStatus::WARNING ? "WARNING" : "FATAL") << "] "
            << description;*/

        oss << "[" << id << "] ["
            << (status == MessageStatus::SUCCESS ? "SUCCESS" :
                status == MessageStatus::WARNING ? "WARNING" : "FATAL") << "] "
            << description;

        return oss.str();
    }
};

// Базовый интерфейс для конкретных логгеров
class ILogger {
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
};

// Класс-менеджер Logger
class Logger {
private:
    std::vector<std::shared_ptr<ILogger>> loggers;
    int idCounter = 1; // Уникальный идентификатор для логов

public:
    void addLogger(const std::shared_ptr<ILogger>& logger) {
        loggers.push_back(logger);
    }
    void log(MessageStatus status, const std::string& description) {
        auto timePoint = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);

        // Создаём буфер для временной строки
        char timeBuffer[26];  // Размер должен быть не менее 26 символов

        // Используем безопасную версию ctime
        errno_t err = ctime_s(timeBuffer, sizeof(timeBuffer), &time);

        // Проверяем успешность операции
        if (err != 0) {
            // Если произошла ошибка, используем резервное значение
            timeBuffer[0] = '\0';
        }

        std::string timeStr(timeBuffer);
        timeStr.pop_back(); // Убираем символ перевода строки

        LogItem item{ idCounter++, timeStr, status, description };

        for (const auto& logger : loggers) {
            if (logger->isEnabled()) {
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

#endif // LOGGER_HPP

