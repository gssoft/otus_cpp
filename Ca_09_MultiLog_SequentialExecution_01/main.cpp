// Ca_09_MultiLog_SequentialExecution_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "SingleExecutor.hpp"

int main() {
    // ������� SingleExecutor
    SingleExecutor executor;

    // ������� MultiLogger, ����������������� SingleExecutor
    MultiLogger<SingleExecutor> multiLogger(executor);

    // ��������� ���������� ������
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->setup_channel_id(1);
    multiLogger.addLogger(consoleLogger);

    // ��������� ������ � ������
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->setup_channel_id(2);
    multiLogger.addLogger(memoryLogger);

    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->setup_channel_id(3);
    fileLogger->set_file_name("logs.txt");
    fileLogger->enable(true);
    multiLogger.addLogger(fileLogger);

    // ���������� ��������� ��������� � ���
    multiLogger.log(MessageStatus::SUCCESS, "Initialization complete");
    multiLogger.log(MessageStatus::WARNING, "Low disk space");
    multiLogger.log(MessageStatus::FATAL, "Unexpected error occurred");

    multiLogger.log(MessageStatus::SUCCESS, "Initialization complete");
    multiLogger.log(MessageStatus::WARNING, "Low disk space");
    multiLogger.log(MessageStatus::FATAL, "Unexpected error occurred");

    // ��������, ����� ���� ������ ����� ���������� �������
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // ������� ��� ���� (������)
    std::cout << "Logs from MemoryLogger: " << std::endl;
    memoryLogger->print();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}



