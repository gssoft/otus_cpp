#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>
#include <iostream>
#include <condition_variable>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

// Перечисление доступных форматов времени
enum class DateTimeFormat {
    WithDate_HH_MM,              // yy.mm.dd hh:mm
    WithDate_HH_MM_SS,           // yy.mm.dd hh:mm:ss
    WithDate_HH_MM_SS_ms,        // yy.mm.dd hh:mm:ss.mmm
    WithDate_HH_MM_SS_us,        // yy.mm.dd hh:mm:ss.uuuuuu
    WithoutDate_HH_MM,           // hh:mm
    WithoutDate_HH_MM_SS,        // hh:mm:ss
    WithoutDate_HH_MM_SS_ms,     // hh:mm:ss.mmm
    WithoutDate_HH_MM_SS_us,     // hh:mm:ss.uuuuuu
};

// Объявление функции форматирования времени
std::string formatDateTime(const DateTimeFormat& format);

// Перечисление уровней логирования
enum class LogLevel {
    SUCCESS,
    WARNING,
    FATAL,
    INFO,
    DEBUG,
    TRACE,
    ERROR,
    EXCEPTION,
    CRITICAL
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
    void Log(const LogItem& item) override;
};

// ----------

// Шаблонный класс Service
template <typename LoggerType>
class Service : public ILogger {
public:
    Service(LoggerType&& logger)
        : logger_(std::forward<LoggerType>(logger))
        , running_(false)
        //, defaultDateTimeFormat(DateTimeFormat::WithDate_HH_MM_SS)
        , defaultDateTimeFormat(DateTimeFormat::WithoutDate_HH_MM_SS_ms)
        , silentMode(false)
        , fatalOnlyMode(false) {
    }
    // 88888 изменения
    // Реализуем метод интерфейса ILogger
    void Log(const LogItem& item) override {
        logger_.Log(item);
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
            AddLogItem(LogLevel::SUCCESS, "LogService", "Start()", "Hello World!");
        }
    }

    void Stop() {
        AddLogItem(LogLevel::SUCCESS, "Service", "Stop()");
        if (running_) {
            AddLogItem(LogLevel::SUCCESS, "Service", "Stop() in process ...");
            running_ = false;
            cv_.notify_one();
            if (worker_thread_.joinable())
                worker_thread_.join();
        }
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

    // Теперь метод `AddLogItem` принимает отдельные компоненты структуры `LogItem`,
    // что позволяет передавать им корректно,
    // избегая ошибок из - за отсутствия полной структуры `LogItem` в вызвонах из `main.cpp`.

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

