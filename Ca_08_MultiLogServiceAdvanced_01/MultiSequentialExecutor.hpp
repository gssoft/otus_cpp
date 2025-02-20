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
    std::unordered_map<int, std::queue<std::function<void()>>> tasks; // ������ ����� �� ID
    std::vector<std::thread> workers;                          // ������� ������
    std::mutex mtx;                                            // ������� ��� �������������
    std::condition_variable cv;                                // �������� ���������� ��� ��������� �����
    bool stop = false;                                         // ���� ���������
    std::atomic<size_t> activeThreads{ 0 };                     // ������� �������� �������

    // ������� ��� ���������� ����� �� ����������� ������
    void workerFunc() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mtx);

                // ���� ������ ��� ������� ���������
                cv.wait(lock, [this] { return stop || hasPendingTasks(); });

                // ���� ����������� � ����� ��� - ���������
                if (stop && !hasPendingTasks())
                    return;

                // ���� ������ � ����� �� �������
                for (auto& [channel_id, queue] : tasks) {
                    if (!queue.empty()) {
                        task = std::move(queue.front());
                        queue.pop();
                        break;
                    }
                }
            }

            // ����������� ������� �������� �������
            activeThreads++;
            task(); // ��������� ������
            activeThreads--; // ��������� ������� ����� ���������� ������
        }
    }

    // ��������� ���������� ������� ���������� ����� � �������
    bool hasPendingTasks() const {
        for (const auto& [channel_id, queue] : tasks) {
            if (!queue.empty()) return true;
        }
        return false;
    }

public:
    explicit MultiSequentialExecutor(size_t threadCount) {
        // ������� �������� ���������� �������
        for (size_t i = 0; i < threadCount; ++i) {
            workers.emplace_back(&MultiSequentialExecutor::workerFunc, this);
        }
    }

    ~MultiSequentialExecutor() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true; // ��������� ����������
        }
        cv.notify_all(); // ���������� ��� ������

        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }

    void execute(int channel_id, const std::function<void()>& task) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks[channel_id].push(task); // ��������� ������ � ��������������� �����
        }
        cv.notify_one(); // ���������� �����
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

