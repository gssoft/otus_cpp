#pragma once

#ifndef LOGSERVICE_HPP
#define LOGSERVICE_HPP

#include "BaseLogger.hpp"
#include "SingleThreadExecutor.hpp"
#include <memory>

// ��������� ����� LogService, ���������� ��������� Logger � Executor
template <typename LoggerType, typename ExecutorType>
class LogService {
public:
    // ����������� ��������� ����� ��������� ��� �������� Logger � Executor
    template <typename... Args>
    LogService(Args&&... args)
        : logger_(std::make_unique<LoggerType>(std::forward<Args>(args)...)),
        executor_(std::make_unique<ExecutorType>(*logger_)) {
    }

    ~LogService() {
        Stop();
    }

    // ��������� ����������� (Executor)
    void Start() {
        executor_->Start();
    }

    // ������������� �����������
    void Stop() {
        if (executor_) {
            executor_->Stop();
        }
    }

    // �����������
    void Log(LogLevel level, const std::string& message) {
        executor_->Log(level, message);
    }

private:
    // ������� Logger � Executor
    std::unique_ptr<BaseLogger> logger_;
    std::unique_ptr<SingleThreadExecutor> executor_;
};

#endif // LOGSERVICE_HPP

