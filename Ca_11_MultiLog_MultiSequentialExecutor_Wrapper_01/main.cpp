// Ca_11_MultiLog_MultiSequentialExecutor_Wrapper_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MultiLoggerMultiThreaded.hpp"
#include "RealLoggers.hpp"

int main() {
    // ������� MultiLoggerMultiThreaded � n SingleExecutor
    MultiLoggerMultiThreaded<MultiLogger<MultiSequentialExecutor<SingleExecutor>>, // MultiLogger
        MultiSequentialExecutor<SingleExecutor>> // MultiSequentialExecutor
        multiLogger_multiThreaded_service(3);

    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->setup_channel_id(3);
    consoleLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(consoleLogger);
    // multiLogger.addLogger(consoleLogger);

    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->setup_channel_id(2);
    memoryLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(memoryLogger);
   


    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->setup_channel_id(1);
    fileLogger->set_file_name("logs.txt");
    fileLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(fileLogger);

    // �������� ��������� ���������
    multiLogger_multiThreaded_service.log(MessageStatus::SUCCESS, "Message 1");
    multiLogger_multiThreaded_service.log(MessageStatus::WARNING, "Message 2");
    multiLogger_multiThreaded_service.log(MessageStatus::FATAL, "Message 3");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // �������� ��
    multiLogger_multiThreaded_service.printAll();

    return 0;
}
/*
����� ��������:

������ ������������ ����� ����������� ������� ����������� � �������������� ��������������� � ��������� "������������ �����" (Executors), ���� �������� ������ �������������� �� ��������� �������. �������� ���� ������� � ������������ ������ � ����������� �������� �����������, ������� ����� ���� ������������� ��� ������������� ���������� � �������������� ���������� ��������� �����������:

    ������� (Loggers):
        ��������� ���� ���������� �������� (��������, ConsoleLogger, MemoryLogger, FileLogger), ������� ��������� ������ ��������� ILogger.
    MultiLogger:
        ����������� �����, ���������� � ����������� ���������. �������� �� ������������� ����� ����������� ����� ����.
    Executor'�:
        ����������� ������������ ���������� ��������� ������������ �����:
            SingleExecutor: ������������ ��� ���������� ����� ��������������� � ����� ������.
            MultiSequentialExecutor: ��������� ������� SingleExecutor ��� ��������� ���������������.
            MultiLoggerMultiThreaded: ������������� �������, ������������ MultiLogger � ������������� ����������.
    �������������� ������� (DateTimeFormatter):
        ������������� ��������� ������� ��� � �������, ������� ����� ������������ ��� ���������� ��������� ����� � ����.

�������� ����������:

    �������:
        ConsoleLogger: ������� ��� ���� � �������.
        MemoryLogger: ��������� ���� � ������ �� ������������� ������.
        FileLogger: ��������� ���� � ����.

������ ������ ����� ���� �������/��������, ����� ����������� ��������� ������� ������� � ������������ ������������� �������������.

    ��������� LogItem:
        ������������ ��� ������������� ����� ������ ����. ���� ��������:
            ���������� ID (id);
            ID ������ (channel_id);
            ����� ������ (time);
            ������ ��������� (SUCCESS, WARNING, FATAL);
            ��������� (description).
    MultiLogger:
        �������� � ����������� ���������, ��������� �� ������ ����������� ����� Executor.
    MultiLoggerMultiThreaded:
        ����������� MultiLogger � ���������� ���������������, ��������������� MultiSequentialExecutor, ��� �������������� ����������� � ���������� �������.
    DateTimeFormatter:
        ������������ ��� �������������� ��������� ����� � ���������� �����.

��������� ����������� (Executors):

��������� ���������� ���������� ������������ ���������:

    SingleExecutor:
        ������������ ������ �� ����� � ����� ����������� ������.
    MultiSequentialExecutor:
        ������ ������ SingleExecutor'�� � ������������ ������ ����� ����.
    MultiLoggerMultiThreaded:
        ����������� MultiSequentialExecutor � MultiLogger ��� ������ � ����������� ��������� � ������������� ������.

����������� �������:

    ��������������� � ������������ �������� �����������:
        ��� ������ ����������� ����������� ���������� ����� ������������� �����������.
        ������� ���������� ������������� ����� ����� ������������� (executors), ��� ��������� �������� � ���������.
    �����������:
        ������ ��������� (������, executor � �.�.) ���������� ��������, �������� ����� ��������� ��� �������� ���� �� �������.
    ����������:
        ������� ���������� ��������� (ILogger), ��� �������� ���������� ����� ����� ��������.
    �������������� �������:
        ��������� ���������� �������� ��������� ����� ��������� ������������ �������� ������ ������.
    ������ ������:
        ������������� std::mutex � std::lock_guard ����������� ������������ ��� ������������� �������.

���-�����: ������� � ������ �������
C������ �������:

    ��������������� (������������� std::mutex � std::jthread):
        ������� ����� ������� ����������� ���������� ��� ������ � ����������������.
        ������������� std::stop_token ��� ���������� ��������� �������.
    ������������:
        MultiLogger � ��� �������������� � Executors ��������, ��� ������ ������� ����� �����������.
    ���������� ������������:
        ������ ����� �������� �� ���� ������ (��������, ������� � �� ���� �����, executors � �� ���������� �����, DateTimeFormatter � �� �����).
        ��� �������� ���������� � ������������� ����.
    ��������:
        ����������� ���������� ����� ����� �������� (��������, ��� ���� ������ ��� ����) ��� ��������� ��������� ������ ����.
    �������������� �������:
        ���������� ������������� ��������� ����� �������� ���������� �����.
    ������������� RAII:
        �������������� ���������� ��������� (��������, �������� ����� � FileLogger).

������ �������/���������:

    ������������� ����������� ���������:
        � ��������� ������ ������������ "������" �������� (��������, � MemoryLogger � FileLogger), ��� ������� �������������� ��������.
    ������������� "������������ �������":
        ��� ������� ���������� ����� ���� ����� �������������� � ���������������� ����� ��� ����� �������� �� ���������. ��� ����� ��������� ����������� ��������� ����������� ��������.
    ������ ��������� ������:
        ��������, ��� �������� ����� � FileLogger, ���� ���� �� ������� �������, ����� ����������� ���������� ��� �������� ����� ����� ��������� �� ������.
    ����-����� � localtime_s:
        ������� localtime_s ������������ ���������, �� ����� ������������� ������������� std::chrono �������� ��������� ����������� �� C-style ������� �������.
    ������������ ��������:
        ��������� channel_id ������� �������� ������� ����������. �������������� ���������� channel_id � ������������ �������������� �������������� ��������� ����� ����� �������.
    ���������� ������������:
        ��� �������� ������ �����������������, �� ����� ������������� ����� ����-������ ��� ��������.
    ���������� ���������� (enabled):
        ���������/���������� �������� (enable(bool)) ����� ������� ����� ��������, ��������� ��������� ���������� (std::atomic<bool>).

������������ �� ���������:

    �������� ������� ����� � ������������ �������:
        ��� MultiLogger �������� ����������� �� ������������ ����� �������.
    �������:
        �������� ����� ����� ���������� channel_id, ��� ������ ��������� (setup_channel_id) ����� ������������.
    �������� ���������� ���������:
        ��������� ����������� ������ ������ ������ ��������� ������� �� �������������.
    ��������� ������:
        ���������, ��� ������ ������ � ������ ��� �������� ��������� �������������� � ����������.
    ���������� ������ � �����/��������:
        ���������� �� localtime_s � ������ std::chrono::zoned_time.
    ����������� �����:
        �������� ������������������ ����� ��� ����������� (SingleExecutor, MultiLogger, �������).
    ������������ �������:
        � ���� ����������� ����������� ��� �������. �������� �� ��� ������� ���������.

����

��� �������� ����� ������� �������� ������������ � ������� ������� � ����������� �������������� �����������.
��� �� �����, ��������� ���������, ����������� ���������� �������� � ��������� ������ ������� ������
����� ���������������� � ����������.
*/