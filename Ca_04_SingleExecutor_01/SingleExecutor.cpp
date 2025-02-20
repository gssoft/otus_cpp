#include "SingleExecutor.hpp"

SingleExecutor::SingleExecutor(BaseLogger& logger)
    : logger_(logger), running_(false) {
}

SingleExecutor::~SingleExecutor() {
    Stop();  // Убедимся, что поток завершён при уничтожении объекта
}

void SingleExecutor::Start() {
    if (!running_) {
        running_ = true;
        workerThread_ = std::jthread(&SingleExecutor::WorkerLoop, this);
    }
}

void SingleExecutor::Stop() {
    if (running_) {
        running_ = false;
        cv_.notify_all();  // Пробуждаем поток
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }
}

void SingleExecutor::Log(LogLevel level, const std::string& message) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        logQueue_.emplace(level, message);
    }
    cv_.notify_one();
}

void SingleExecutor::WorkerLoop() {
    while (running_) {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this] { return !logQueue_.empty() || !running_; });

        if (!running_ && logQueue_.empty()) {
            return;  // Прерываем выполнении, поток завершён
        }

        auto [level, message] = logQueue_.front();
        logQueue_.pop();
        lock.unlock();

        logger_.Log(level, message);  // Логируем сообщение
    }
}
