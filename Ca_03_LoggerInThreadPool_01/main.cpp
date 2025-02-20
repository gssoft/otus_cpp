// Ca_03_LoggerInThreadPool_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#include "LogService.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <thread>
#include <chrono>

// ConsoleLogger
// Service c одним thread (поток исполнения) параметризирован ConsoleLogger
// Получаетcя ConsoleLogger работающий с предоставленным одним Thread от Service
// Демонстрируется работа log_service
//  - в контексте основной программы main()
//  - в контексте THreadPool - логировать используя только свой поток и не использовать внутренние потоки ThreadPool
// 

int main() {

    // setlocale(LC_ALL, "RU");

    ConsoleLogger consoleLogger;
    Service<ConsoleLogger> log_service(std::move(consoleLogger));
    ThreadPool threadPool(log_service);

    log_service.AddLogItem(LogLevel::SUCCESS, "main()", "Start Application", "Hello World");
    log_service.Start();

    // Пример использования пула потоков
    threadPool.enqueue_sequential([&log_service]() {
        log_service.AddLogItem(LogLevel::SUCCESS, "ThreadPool", "Test message from Sequential ThreadPool");
        });
    // Пример использования логирования
    log_service.AddLogItem(LogLevel::SUCCESS, "main()", "Log_Service", "Test message");
    log_service.AddLogItem(LogLevel::WARNING, "main()", "Log_Service", "Warning message");
    log_service.AddLogItem(LogLevel::FATAL, "main()", "Log_Service", "Fatal error: Something went wrong");

    log_service.AddLogItem(LogLevel::SUCCESS, "main()", "Application in the process of completion ...", "Good Bye");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // log_service.AddLogItem(LogLevel::SUCCESS, "main()", "Application to be completed ...", "Good Bye");
    log_service.Stop();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}

/*
В окне консоли видно когды мы работаем внутри ThreadPool, а когда в main().
*/


