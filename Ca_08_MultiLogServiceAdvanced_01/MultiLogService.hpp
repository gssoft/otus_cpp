#pragma once

#include "MultiLogger.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

// Базовый интерфейс исполнения задач
class IExecutor {
public:
    virtual ~IExecutor() = default;
    virtual void execute(const std::function<void()>& task) = 0;
};

// Простая однопоточная реализация исполнителя
class SingleThreadExecutor : public IExecutor {
private:
    std::queue<std::function<void()>> tasks;
    std::thread worker;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop = false;

    void run() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this] { return stop || !tasks.empty(); });

                if (stop && tasks.empty())
                    return;

                task = std::move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }

public:
    SingleThreadExecutor() : worker(&SingleThreadExecutor::run, this) {}

    ~SingleThreadExecutor() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();
        if (worker.joinable())
            worker.join();
    }

    void execute(const std::function<void()>& task) override {
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.push(task);
        }
        cv.notify_one();
    }
};

// Шаблонный класс, связывающий MultiLogger и Executor
template <typename LoggerType, typename ExecutorType>
class MultiLogService {
private:
    LoggerType logger;
    ExecutorType executor;

public:
    void addLogger(const std::shared_ptr<ILogger>& loggerInstance) {
        logger.addLogger(loggerInstance);
    }

    void log(MessageStatus status, const std::string& description) {
        executor.execute([this, status, description]() {
            logger.log(status, description);
            });
    }

    void printAll() {
        executor.execute([this]() {
            logger.printAll();
            });
    }
};


