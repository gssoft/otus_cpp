#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <iostream>

template <typename Executor>
class MultiSequentialExecutor {
public:
    explicit MultiSequentialExecutor(size_t numExecutors) {
        executors_.reserve(numExecutors);
        for (size_t i = 0; i < numExecutors; ++i) {
            executors_.emplace_back(std::make_unique<Executor>());
        }
    }

    ~MultiSequentialExecutor() {
        // Уничтожение ресурсов происходит через деструкторы Executor
    }

    // Метод для добавления задачи
    void SubmitTask(size_t id, std::function<void()> task) {
        // Распределение задачи по исполнителю
        size_t executorIndex = id % executors_.size();
        executors_[executorIndex]->post(std::move(task));
    }

private:
    std::vector<std::unique_ptr<Executor>> executors_; // Вектор исполнителей
};

