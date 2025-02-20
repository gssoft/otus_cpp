// Service.hpp
#pragma once

#include "FormatDateTime.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>
#include <condition_variable>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <iostream> 

using namespace std;

// Перечисление уровней логирования
enum class LogLevel {
    SUCCESS,
    WARNING,
    FATAL
};

// Структура для записи лога
struct LogItem {
    DateTimeFormat dateTimeFormat;
    LogLevel option;
    std::string param1;
    std::string param2;
    std::string param3;
};

// Интерфейс для логгера
class ILogger {
public:
    virtual ~ILogger() {}  // Добавляем виртуальный деструктор
    virtual void Log(const LogItem& item) = 0;
};

// Консольный логгер
class ConsoleLogger : public ILogger {
public:
    void Log(const LogItem& item) override {
        std::string timeString = formatDateTime(item.dateTimeFormat);

        switch (item.option) {
        case LogLevel::SUCCESS:
            std::cout << "[" << timeString << "] [SUCCESS] ";
            break;
        case LogLevel::WARNING:
            std::cout << "[" << timeString << "] [WARNING] ";
            break;
        case LogLevel::FATAL:
            std::cout << "[" << timeString << "] [FATAL] ";
            break;
        }

        std::cout << item.param1;
        if (!item.param2.empty()) {
            std::cout << " " << item.param2;
        }
        if (!item.param3.empty()) {
            std::cout << " " << item.param3;
        }
        std::cout << std::endl;
    }
};

// Шаблонный класс Service
template <typename LoggerType>
class Service {
public:
    Service(LoggerType&& logger)
        : logger_(std::forward<LoggerType>(logger))
        , running_(false)
        , defaultDateTimeFormat(DateTimeFormat::WithDate_HH_MM_SS)
        , silentMode(false)
        , fatalOnlyMode(false) {
    }

    void SetDateFormatDefault(DateTimeFormat format) {
        defaultDateTimeFormat = format;
    }

    void SetSilentMode(bool mode) {
        silentMode = mode;
    }

    void SetFatalOnlyMode(bool mode) {
        fatalOnlyMode = mode;
    }

    void Start() {
        if (!running_) {
            running_ = true;
            worker_thread_ = std::thread(&Service::WorkerLoop, this);

            // Логируем событие старта сервиса
            // AddLogItem({ defaultDateTimeFormat, LogLevel::SUCCESS, "Service", "Start()"});
            AddLogItem(LogLevel::SUCCESS, "Service", "Start()");
        }
    }

    void Stop() {
        AddLogItem(LogLevel::SUCCESS, "Service", "Stop()");
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (running_) {
            // AddLogItem(LogLevel::SUCCESS, "Service", "Stop()");
            running_ = false;
            cv_.notify_one();  // Убедимся, что поток проснется
            if (worker_thread_.joinable())
                worker_thread_.join();
        }
        //else
        //    AddLogItem(LogLevel::SUCCESS, "Service", "Stop()");
    }
    void AddLogItem(LogLevel level, const std::string& param1, const std::string& param2 = "", const std::string& param3 = "") {
        if (silentMode) {
            return;
        }

        if (fatalOnlyMode && level != LogLevel::FATAL) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        LogItem item;
        item.dateTimeFormat = defaultDateTimeFormat;
        item.option = level;
        item.param1 = param1;
        if (!param2.empty()) {
            item.param2 = param2;
        }
        if (!param3.empty()) {
            item.param3 = param3;
        }
        log_queue_.push(item);
        cv_.notify_one();
    }

private:
    void WorkerLoop() {
        while (running_) {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [this] { return !log_queue_.empty() || !running_; });

            if (!running_) {
                break;
            }

            LogItem item = log_queue_.front();
            log_queue_.pop();
            lock.unlock();

            logger_.Log(item);
        }
    }

    std::queue<LogItem> log_queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::thread worker_thread_;
    LoggerType logger_;
    std::atomic_bool running_;
    DateTimeFormat defaultDateTimeFormat;
    bool silentMode;
    bool fatalOnlyMode;
};


