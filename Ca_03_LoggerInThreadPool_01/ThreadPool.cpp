#include "ThreadPool.hpp"

void ThreadPool::sequential_worker() {
    while (sequential_running_) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(sequential_mutex_);
            sequential_cv_.wait(lock, [this] {
                return !sequential_queue_.empty() || !sequential_running_;
                });

            if (!sequential_running_ && sequential_queue_.empty()) {
                AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Worker thread stopping");
                return;
            }

            if (!sequential_queue_.empty()) {
                AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Worker processed task\n");
                task = std::move(sequential_queue_.front());
                sequential_queue_.pop();
            }
        }
        if (task) {
            try {
                task();
            }
            catch (const std::exception& e) {
                AddLogItem(LogLevel::FATAL, "ThreadPool", "Task execution failed", e.what());
            }
        }
    }
}
