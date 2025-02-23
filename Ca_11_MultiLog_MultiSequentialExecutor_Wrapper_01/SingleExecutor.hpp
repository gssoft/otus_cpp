#pragma once
#ifndef SINGLE_EXECUTOR_HPP
#define SINGLE_EXECUTOR_HPP

#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <stop_token>
#include <thread>
#include <iostream>

class SingleExecutor {
public:
	SingleExecutor() : done(false), worker(&SingleExecutor::work, this, worker.get_stop_token()) {}

	~SingleExecutor() {
		{
			std::lock_guard<std::mutex> lock(mtx);
			done = true;
			cond.notify_all();
		}
	}

	void post(std::function<void()> task) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			tasks.push(std::move(task));
		}
		cond.notify_one();
	}

private:
	void work(std::stop_token stop_token) {
		while (!stop_token.stop_requested()) { // Проверяет, запрошено ли завершение работы
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(mtx);
				cond.wait(lock, stop_token, [&]() { return done || !tasks.empty(); });

				if ((done && tasks.empty()) || stop_token.stop_requested())
					break;

				task = std::move(tasks.front());
				tasks.pop();
			}

			if (task) task();
		}
	}
	std::queue<std::function<void()>> tasks;
	std::mutex mtx;
	std::condition_variable_any cond; // Условная переменная Any для работы с std::stop_token
	bool done;
	std::jthread worker; // std::jthread вместо std::thread
};

#endif // SINGLE_EXECUTOR_HPP
