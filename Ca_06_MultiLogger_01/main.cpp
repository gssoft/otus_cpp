// Ca_06_MultiLogger_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Logger.hpp"
#include "RealLoggers.hpp"

/*
    ���� � ���������� ������� �� ������������ �� ���� ->
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    � ��� ���� ConsoleLogger  � ����������� � ����� �������.

    ��� ����� ���������������� ����� ������ ConsoleLogger-� �������� �����.
    ����� ��� ���� �������
        -   MemoryLogger (Cash) - �� ������������ ���-�� �������, ����� ��� ����� ����� ���� ����������
            N - ��������� Log ��������, � ��������� ��������� 100 �
        -   FileLogger - ����� ���������� ��� ������ � ������������ ����. ��� ����� �������� � ����. �� ������ 49

    ����� ������� � ��� �����:
    Logger - �������� �
    Loggers:
        - ConsoleLogger
        - MemoryLogger
        - FileLogger
*/

// ���������� � ���� �������

int main() {
    // ������� �������� �������� �����
    Logger logger;

    // ��������� ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    logger.addLogger(consoleLogger);

    // ��������� MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    memoryLogger->set_max_logitems(100);   // MAX ���������� ��������� ������� � CASH-�.
    logger.addLogger(memoryLogger);

    // ��������� FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    fileLogger->set_dt_format(DateTimeFormat::WithDate_HH_MM_SS_ms);
    logger.addLogger(fileLogger);

    // �������� ��� ���������
    logger.log(MessageStatus::SUCCESS, "Hello world");
    logger.log(MessageStatus::WARNING, "Good bye");

    // ������� ��� ���� ��� �������� ��������
    logger.printAll();

    logger.log(MessageStatus::SUCCESS, "Hello world");
    logger.log(MessageStatus::WARNING, "Good bye");

    // ������� ��� ���� ��� �������� ��������
    logger.printAll();

    return 0;
}

