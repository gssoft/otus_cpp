// Ca_07_MultiLogService_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Ca_MultiLogService_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
    В предыдущих проектах мы разработали новый Logger c отдельными Loggers:
    - ConsoleLOgger
    - MemoryLogger
    - FileLogger

    а также мы имеем вот такую кострукцию от другого предыдущего проекта
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    Что мы сделаем в этом проекте
    - Переименуем наш Логгер и дадим ему новое гордое название MultiLogger
    - Переименуем наш сервис из LogService в MultiLogService
    - И опять сделаем ту же ШАБЛОННУЮ магию.
    Вот так
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;
*/
/*
    Таким образом у нас было так:
        LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;
    Станет так:
        MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    и колонку я подправил
*/

#include <iostream>
#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "MultiLogService.hpp"

int main() {
    // Создаем MultiLogService, связывая MultiLogger и SingleThreadExecutor в Wrapper c именем MultiLogService
    // 
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    // Добавляем ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    multi_log_service.addLogger(consoleLogger);

    // Добавляем MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    memoryLogger->set_max_logitems(100);
    multi_log_service.addLogger(memoryLogger);

    // Добавляем FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    fileLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    multi_log_service.addLogger(fileLogger);

    // Логируем два сообщения
    multi_log_service.log(MessageStatus::SUCCESS, "Hello world");
    multi_log_service.log(MessageStatus::WARNING, "Good bye");
    multi_log_service.log(MessageStatus::FATAL, "Somethings goes Baaaaaaad!");

    // Выводим все логи
    multi_log_service.printAll();

    return 0;
}




