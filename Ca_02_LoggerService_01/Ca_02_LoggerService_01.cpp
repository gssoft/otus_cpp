// Ca_02_LoggerService_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
// ������ 02

#include "FormatDateTime.hpp"
#include "Service.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// ���������� ConsoleLogger
// ConsoleLogger �������� � ��������� Service � ����� ������� ����������
// 

int main() {
    ConsoleLogger console_logger;
    Service<ConsoleLogger> console_log_service(std::move(console_logger));

    // ������������� ������ ���� �� ���������
    console_log_service.SetDateFormatDefault(DateTimeFormat::WithDate_HH_MM_SS);

    // On/Off ����� ����������� ������ ����������� ���������
    console_log_service.SetFatalOnlyMode(false);
    console_log_service.SetSilentMode(false);

    console_log_service.Start();

    console_log_service.AddLogItem(LogLevel::SUCCESS, "Message 1");
    console_log_service.AddLogItem(LogLevel::WARNING, "Message 2", "Param 2");
    console_log_service.AddLogItem(LogLevel::FATAL, "Message 3", "Param 2", "Param 3");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    // ���� �������, ����� ���� ����������� ������ ���������� ��� ���������
    // ��������. 
    // ������ �������� ��� �������� � main().
    // ������� �������� � Stop() - ��� ��� ����� �������
    console_log_service.Stop();

    return 0;
}

