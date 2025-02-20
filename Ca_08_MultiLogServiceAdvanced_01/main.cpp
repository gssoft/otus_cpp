// Ca_08_MultiLogServiceAdvanced_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "MultiLogServiceAdvanced.hpp"
#include "MultiSequentialExecutor.hpp"

/*
    � ���������� ������� �� ������������ ��� �� ����
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    �� ����� ���� ���������� ���������� ���������� � �������������� ���� (��� ����� ����� �� ���������� ������)
    � �������, ��� ��� ���������.
    ��� ������ - ������ � ����� ������� ������� � �������� ����������� ����� ���-������.
    ��������� Loggers (FileLogger � ConsoleLogger) ����� ��������� �������.
    ��� �����, ��� ���� ��� DataBaseLogger, HttpLogger, GrpcLogger ������� - ������, ��� �����.
    � ��� ���� ����� ���������������� ������� ���-������� ���.
    ������� ��� ���� ������������ <SingleThreadExecutor>  �������� �� �������������� MultiSequentialExecutor.
    �������� �������
    ���������� ����. ������ ��������� ����� ��������� ����� �� ������ �������������� ���������.
    �� �������� ������������ wrapper c MultiLogService �� MultiLogServiceAdvanced
    � ����������� ����� Executor: MultiSequentialExecutor - �������������� ���������������� Executor.
    ������� ������� �������� ���� ����������� ���������������� ����� ��� ������������� ��������� �
    ��� ���������� ����������������� ������� ���-�������.
*/

int main() {
    // ������� MultiLogServiceAdvanced � ������������� ���������������� ������������
    // ���� ���:
    // MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;
    // ����� ���:
    MultiLogServiceAdvanced<MultiLogger, MultiSequentialExecutor> multi_log_service_adv(3); // 3 ������. 

    // ��������� ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    // consoleLogger->enable(false);
    consoleLogger->setup_channel_id(1); // ����� 1
    multi_log_service_adv.addLogger(consoleLogger);

    // ��������� MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    // memoryLogger->enable(false);
    memoryLogger->setup_channel_id(2); // ����� 2
    multi_log_service_adv.addLogger(memoryLogger);

    // ��������� FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    // fileLogger->enable(false);
    fileLogger->setup_channel_id(3); // ����� 3
    multi_log_service_adv.addLogger(fileLogger);

    // �������� ��������� ���������

    multi_log_service_adv.log(1, MessageStatus::SUCCESS, "Message1 on channel 1");
    multi_log_service_adv.log(1, MessageStatus::WARNING, "Message2 on channel 2");
    multi_log_service_adv.log(1, MessageStatus::FATAL, "Message3 on channel 3");

    // �������� ��� ����
    multi_log_service_adv.printAll();

    // ���������� ��������� �����������
    // to Do
  //   std::cout << "Active threads: " << multi_log_service_adv.getActiveThreadCount() << "\n";
  //   std::cout << "Pending tasks: " << multi_log_service_adv.getTaskQueueSize() << "\n";

    return 0;
}

/*
    ����� ������� �� ������ ���� �� ������ ������������ ConsoleLogger �� MultoLogService,
    ����������� � ���������������� ��������������� ThreadPool.
*/

/*

��������� ��������.
���������� ����� ��������� ��������� ����������.
��������� ����� �������:
������ � �������� ��������.
������� �������������� �������
*/

