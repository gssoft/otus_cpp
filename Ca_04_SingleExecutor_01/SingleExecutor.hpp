#pragma once

#ifndef SINGLE_EXECUTOR_HPP
#define SINGLE_EXECUTOR_HPP

#include "BaseLogger.hpp"
#include <queue>
#include <thread>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <string>

class SingleExecutor {
public:
    explicit SingleExecutor(BaseLogger& logger);
    ~SingleExecutor();

    // Логирование через SingleExecutor
    void Log(LogLevel level, const std::string& message);

    // Управление процессом обработки
    void Start();
    void Stop();

private:
    void WorkerLoop();

    BaseLogger& logger_;
    std::queue<std::pair<LogLevel, std::string>> logQueue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::jthread workerThread_;
    std::atomic_bool running_;
};

#endif // SINGLE_EXECUTOR_HPP

