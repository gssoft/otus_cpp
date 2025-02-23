// Ca_06_MultiLogger_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logger.hpp"
#include "RealLoggers.hpp"
#include <thread>

/*
    Итак в предыдущем проекте мы остановились на этом ->
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    У нас есть ConsoleLogger  и Исполнитель с одним потоком.

    Для наших широкомасштабных задач только ConsoleLogger-а маловато будет.
    Хотим еще пару логеров
        -   MemoryLogger (Cash) - на определенное кол-во записей, чтобы нам легко можно было посмотреть
            N - последних Log запписей, с дефолтным значением 100 и
        -   FileLogger - будет записывать все записи в определенный файл. Имя файла задается в коде. см строку 49

    Таким образом у нас будет:
    Logger - менеджер и
    Loggers:
        - ConsoleLogger
        - MemoryLogger
        - FileLogger
*/

// реализация в этом проекте

int main() {
    // Создаем основной менеджер логов
    Logger logger;

    // Добавляем ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    logger.addLogger(consoleLogger);

    // Добавляем MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    memoryLogger->set_max_logitems(100);   // MAX КОЛИЧЕСТВО ПОСЛЕДНИХ ЗАПИСЕЙ в CASH-е.
    logger.addLogger(memoryLogger);

    // Добавляем FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    fileLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    logger.addLogger(fileLogger);

    // Логируем два сообщения
    logger.log(MessageStatus::SUCCESS, "Hello world");
    logger.log(MessageStatus::WARNING, "Good bye");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Выводим все логи для активных логгеров
    logger.printAll();

    logger.log(MessageStatus::SUCCESS, "Hello world");
    logger.log(MessageStatus::WARNING, "Good bye");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Выводим все логи для активных логгеров
    logger.printAll();

    return 0;
}

