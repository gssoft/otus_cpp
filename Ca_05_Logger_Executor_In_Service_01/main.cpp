// Ca_05_Logger_Executor_In_Service_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Ca_SingleThreadExecutor_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "LogService.hpp"
#include "ConsoleLogger.hpp"
#include "SingleThreadExecutor.hpp"

    // ���������� �������� � ���������

    // � ������ ������ ������ ���������� �������������� ���� ����������� � �����
    // ���������� Logger(���������) � Executor(����������) � ���� Wrapper � ������ LogService
    // ����� ��������� �����
    // ������ ����� � ������������� ����� ������ � ��������� � ����������
    // ��� ������ ������ ������������� �������.

    /*
    * ���� ��� ���:
    ConsoleLogger logger;  // ���������� �������
    SingleExecutor executor(logger);  // ������������ ����������� (Executor)
    */
    /*
    ������ ��� ���
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    ����� ��� ��������� �����
    */

int main() {

    // setlocale(LC_ALL, "RU");

    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    log_sevice.Start();
    /*
    log_sevice.Log(LogLevel::SUCCESS, "���������� �����.");
    log_sevice.Log(LogLevel::INFO, "��� �������������� ���������.");
    log_sevice.Log(LogLevel::WARNING, "���-�� ���������...");
    log_sevice.Log(LogLevel::FATAL, "Sos. ������� ���� ����!");
    */
    log_sevice.Log(LogLevel::SUCCESS, "Application started.");
    log_sevice.Log(LogLevel::INFO, "This is information message.");
    log_sevice.Log(LogLevel::WARNING, "It's seem to be something wrong...");
    log_sevice.Log(LogLevel::FATAL, "Fatal mtssage");

    std::this_thread::sleep_for(std::chrono::seconds(1));  //

    log_sevice.Stop();
    return 0;
}

