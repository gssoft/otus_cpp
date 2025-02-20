// Ca_02_LoggerService_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Проект 02

#include "FormatDateTime.hpp"
#include "Service.hpp"
#include <iostream>
#include <thread>
#include <chrono>

// Реализация ConsoleLogger
// ConsoleLogger работает в контексте Service с одним потоком выполнения
// 

int main() {
    ConsoleLogger console_logger;
    Service<ConsoleLogger> console_log_service(std::move(console_logger));

    // Устанавливаем формат даты по умолчанию
    console_log_service.SetDateFormatDefault(DateTimeFormat::WithDate_HH_MM_SS);

    // On/Off режим логирования только критических сообщений
    console_log_service.SetFatalOnlyMode(false);
    console_log_service.SetSilentMode(false);

    console_log_service.Start();

    console_log_service.AddLogItem(LogLevel::SUCCESS, "Message 1");
    console_log_service.AddLogItem(LogLevel::WARNING, "Message 2", "Param 2");
    console_log_service.AddLogItem(LogLevel::FATAL, "Message 3", "Param 2", "Param 3");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    // Ждем секунду, чтобы дать возможность потоку обработать все сообщения
    // Исправил. 
    // Сейчас работает без задержки в main().
    // Перенес задержку в Stop() - там она более уместна
    console_log_service.Stop();

    return 0;
}

