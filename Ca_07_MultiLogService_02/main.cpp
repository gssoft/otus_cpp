// Ca_07_MultiLogService_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Ca_MultiLogService_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
/*
    � ���������� �������� �� ����������� ����� Logger c ���������� Loggers:
    - ConsoleLOgger
    - MemoryLogger
    - FileLogger

    � ����� �� ����� ��� ����� ���������� �� ������� ����������� �������
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    ��� �� ������� � ���� �������
    - ����������� ��� ������ � ����� ��� ����� ������ �������� MultiLogger
    - ����������� ��� ������ �� LogService � MultiLogService
    - � ����� ������� �� �� ��������� �����.
    ��� ���
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;
*/
/*
    ����� ������� � ��� ���� ���:
        LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;
    ������ ���:
        MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    � ������� � ���������
*/

#include <iostream>
#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "MultiLogService.hpp"

int main() {
    // ������� MultiLogService, �������� MultiLogger � SingleThreadExecutor � Wrapper c ������ MultiLogService
    // 
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    // ��������� ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    multi_log_service.addLogger(consoleLogger);

    // ��������� MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    memoryLogger->set_max_logitems(100);
    multi_log_service.addLogger(memoryLogger);

    // ��������� FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    fileLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    multi_log_service.addLogger(fileLogger);

    // �������� ��� ���������
    multi_log_service.log(MessageStatus::SUCCESS, "Hello world");
    multi_log_service.log(MessageStatus::WARNING, "Good bye");
    multi_log_service.log(MessageStatus::FATAL, "Somethings goes Baaaaaaad!");

    // ������� ��� ����
    multi_log_service.printAll();

    return 0;
}




