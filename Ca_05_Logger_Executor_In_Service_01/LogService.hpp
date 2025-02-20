#pragma once

#ifndef LOGSERVICE_HPP
#define LOGSERVICE_HPP

#include "BaseLogger.hpp"
#include "SingleThreadExecutor.hpp"
#include <memory>

// Шаблонный класс LogService, обобщающий поведение Logger и Executor
template <typename LoggerType, typename ExecutorType>
class LogService {
public:
    // Конструктор принимает любые аргументы для создания Logger и Executor
    template <typename... Args>
    LogService(Args&&... args)
        : logger_(std::make_unique<LoggerType>(std::forward<Args>(args)...)),
        executor_(std::make_unique<ExecutorType>(*logger_)) {
    }

    ~LogService() {
        Stop();
    }

    // Запускает исполнителя (Executor)
    void Start() {
        executor_->Start();
    }

    // Останавливает исполнителя
    void Stop() {
        if (executor_) {
            executor_->Stop();
        }
    }

    // Логирование
    void Log(LogLevel level, const std::string& message) {
        executor_->Log(level, message);
    }

private:
    // Объекты Logger и Executor
    std::unique_ptr<BaseLogger> logger_;
    std::unique_ptr<SingleThreadExecutor> executor_;
};

#endif // LOGSERVICE_HPP

