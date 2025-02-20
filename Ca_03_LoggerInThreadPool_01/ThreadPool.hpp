#pragma once

#include "LogService.hpp"

#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <utility>

class ILogger;

class ThreadPool {
private:
    std::thread sequential_thread_;                // Отдельный поток для задач
    std::queue<std::function<void()>> sequential_queue_;    // Очередь задач
    std::mutex sequential_mutex_;                  // Мьютекс для защиты доступа к очереди
    std::condition_variable sequential_cv_;        // Условная переменная для управления очередью
    bool sequential_running_ = true;               // Флаг работы потока
    ILogger& logger_;                              // Логгер

    void sequential_worker();                      // Функция-рабочий, выполняет задачи из очереди

public:
    ThreadPool(ILogger& logger) : logger_(logger) {
        AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Created", "Good Luck!");
        sequential_thread_ = std::thread(&ThreadPool::sequential_worker, this);
    }
    // Конструктор для Service<ConsoleLogger>
    ThreadPool(Service<ConsoleLogger>& logService) : logger_(logService) {
        AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Created", "Good Luck!");
        sequential_thread_ = std::thread(&ThreadPool::sequential_worker, this);
    }

    ~ThreadPool() {
        AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Destroying", "Good buy");
        {
            std::lock_guard<std::mutex> lock(sequential_mutex_);
            sequential_running_ = false;
        }
        sequential_cv_.notify_one();
        if (sequential_thread_.joinable()) {
            sequential_thread_.join();
        }
    }

    void enqueue_sequential(std::function<void()> func) {
        {
            std::lock_guard<std::mutex> lock(sequential_mutex_);
            sequential_queue_.push(std::move(func));
        }
        sequential_cv_.notify_one();
        AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Task enqueued");
        AddLogItem(LogLevel::EXCEPTION, "ThreadPool", "Task enqueued");
        AddLogItem(LogLevel::WARNING, "ThreadPool", "Task enqueued");
    }

private:
    void AddLogItem(LogLevel level, const std::string& param1,
        const std::string& param2 = "", const std::string& param3 = "") {
        LogItem item;
        item.dateTimeFormat = DateTimeFormat::WithDate_HH_MM_SS;
        item.option = level;
        item.param1 = param1;
        if (!param2.empty()) item.param2 = param2;
        if (!param3.empty()) item.param3 = param3;
        logger_.Log(item);
    }
};


