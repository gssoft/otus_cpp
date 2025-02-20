#pragma once

#include "MultiLogger.hpp"
#include <functional>
#include <vector>

template <typename LoggerType, typename ExecutorType>
class MultiLogServiceAdvanced {
private:
    LoggerType logger;         // Ёкземпл€р логгера
    ExecutorType executor;     // Ёкземпл€р исполнител€

public:
    explicit MultiLogServiceAdvanced(size_t threadCount)
        : executor(threadCount) {
    }

    // ƒобавление логгера
    void addLogger(const std::shared_ptr<ILogger>& loggerInstance) {
        logger.addLogger(loggerInstance);
    }

    // Ћогирование сообщени€
    void log(int channel_id, MessageStatus status, const std::string& description) {
        executor.execute(channel_id, [this, channel_id, status, description]() {
            logger.log(status, description);
            });
    }

    // ¬ывод всех логов

    void printAll() {
        executor.execute(0, [this]() { // ”правл€ющий канал 0
            logger.printAll();
            });
    }

    /*
    void printAll() {
            logger.printAll();
    }
    */

    size_t getActiveThreadCount() const {
        return executor.getActiveThreadCount();
    }

    size_t getTaskQueueSize() {
        return executor.getTaskQueueSize();
    }
};


