// Ca_01_ConsoleLogger_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
// В начале было слово ...
// А у нас в начале был ConsoleLogger
// совсем простой как в этом проекте.
// Он логирует разные сообщения.
// В main() все написано.

#include "ConsoleLogger.hpp"

int main() {
    ConsoleLogger logger;

    // Пример использования ConsoleLogger
    logger.Log(DateTimeFormat::WithDate_HH_MM_SS, LogLevel::SUCCESS, "Application", "started successfully");
    logger.Log(DateTimeFormat::WithDate_HH_MM, LogLevel::WARNING, "Low memory warning");
    logger.Log(DateTimeFormat::WithDate_HH_MM_SS_ms, LogLevel::FATAL, "Application", "critical error", "shutting down");

    return 0;
}

