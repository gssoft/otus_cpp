// Ca_04_SingleExecutor_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SingleExecutor.hpp"
#include "ConsoleLogger.hpp"

// ���������� SingleExecutor - ����������� � ����� �������
// ���������� ConsoleLogger - ����� ��� ����������� ��������� � �������
// �������� ��������� (ConsoleLogger) � ���������� (SingleExecutor)

int main() {
    ConsoleLogger logger;  // ���������� �������
    SingleExecutor executor(logger);  // ������������ ����������� (Executor)

    executor.Start();

    executor.Log(LogLevel::SUCCESS, "Application started.");
    executor.Log(LogLevel::INFO, "This is an informational message.");
    executor.Log(LogLevel::WARNING, "A warning occurred.");
    executor.Log(LogLevel::FATAL, "Fatal error encountered!");

    std::this_thread::sleep_for(std::chrono::seconds(1));  // ������� ����������

    executor.Stop();
    return 0;
}

