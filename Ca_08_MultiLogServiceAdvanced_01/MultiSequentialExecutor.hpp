#pragma once

#include <queue>
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

class MultiSequentialExecutor {
private:
    std::unordered_map<int, std::queue<std::function<void()>>> tasks; // Каналы задач по ID
    std::vector<std::thread> workers;                          // Рабочие потоки
    std::mutex mtx;                                            // Мьютекс для синхронизации
    std::condition_variable cv;                                // Условная переменная для обработки задач
    std::atomic_bool stop = false;                                         // Флаг остановки
    std::atomic<size_t> activeThreads{ 0 };                     // Подсчет активных потоков

    // Функция для выполнения задач из конкретного канала
    void workerFunc() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);

                // Ждем задачи или сигнала остановки
                cv.wait(lock, [this] { return stop || hasPendingTasks(); });

                // Если остановлено и задач нет - завершаем
                if (stop && !hasPendingTasks())
                    return;

                // Ищем задачу в любом из каналов
                for (auto& [channel_id, queue] : tasks) {
                    if (!queue.empty()) {
                        task = std::move(queue.front());
                        queue.pop();
                        break;
                    }
                }
            }

            // Увеличиваем счетчик активных потоков
            activeThreads++;
            task(); // Выполняем задачу
            activeThreads--; // Уменьшаем счетчик после выполнения задачи
        }
    }

    // Проверяет соблюдение условий выполнения задач в каналах
    bool hasPendingTasks() const {
        for (const auto& [channel_id, queue] : tasks) {
            if (!queue.empty()) return true;
        }
        return false;
    }

public:
    explicit MultiSequentialExecutor(size_t threadCount) {
        // Создаем заданное количество потоков
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back(&MultiSequentialExecutor::workerFunc, this);
        }
    }

    ~MultiSequentialExecutor() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true; // Завершаем выполнение
        }
        cv.notify_all(); // Пробуждаем все потоки

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void execute(int channel_id, const std::function<void()>& task) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks[channel_id].push(task); // Добавляем задачу в соответствующий канал
        }
        cv.notify_one(); // Пробуждаем поток
    }

    size_t getActiveThreadCount() const {
        return activeThreads.load();
    }

    size_t getTaskQueueSize() {
        std::unique_lock<std::mutex> lock(mtx);
        size_t total = 0;
        for (const auto& [channel_id, queue] : tasks) {
            total += queue.size();
        }
        return total;
    }
};

