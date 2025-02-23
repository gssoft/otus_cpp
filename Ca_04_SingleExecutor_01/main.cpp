// Ca_04_SingleExecutor_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SingleExecutor.hpp"
#include "ConsoleLogger.hpp"

// Реализация SingleExecutor - Исполнитель с одним потоком
// Реализация ConsoleLogger - Логер для регистрации сообщений в Консоли
// Разделим Поведение (ConsoleLogger) и Исполнение (SingleExecutor)

int main() {
    ConsoleLogger logger;  // Реализация логгера
    SingleExecutor executor(logger);  // Однопоточный исполнитель (Executor)

    executor.Start();

    executor.Log(LogLevel::SUCCESS, "Application started.");
    executor.Log(LogLevel::INFO, "This is an informational message.");
    executor.Log(LogLevel::WARNING, "A warning occurred.");
    executor.Log(LogLevel::FATAL, "Fatal error encountered!");

    std::this_thread::sleep_for(std::chrono::seconds(1));  // Подождём выполнения

    executor.Stop();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}

