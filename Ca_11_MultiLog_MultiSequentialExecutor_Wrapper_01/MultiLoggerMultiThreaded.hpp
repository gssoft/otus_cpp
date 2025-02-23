#pragma once

#include "MultiLogger.hpp"
#include "MultiSequentialExecutor.hpp"
#include "SingleExecutor.hpp"

// MultiLoggerMultiThreaded
template<typename Logger, typename Executor>
class MultiLoggerMultiThreaded {
private:
    Logger multiLogger; // Внутренний объект MultiLogger
    Executor multiExecutor; // Внутренний MultiSequentialExecutor

public:
    // Конструктор
    MultiLoggerMultiThreaded(size_t numExecutors)
        : multiExecutor(numExecutors), multiLogger(multiExecutor) {
    }

    // Добавление логгера
    void addLogger(const std::shared_ptr<ILogger>& logger) {
        multiLogger.addLogger(logger);
    }

    // Логирование
    void log(MessageStatus status, const std::string& description) {
        multiLogger.log(status, description);
    }

    // Печать всех логов
    void printAll() const {
        multiLogger.printAll();
    }
};


