// Ca_05_Logger_Executor_In_Service_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Ca_SingleThreadExecutor_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "LogService.hpp"
#include "ConsoleLogger.hpp"
#include "SingleThreadExecutor.hpp"

    // Скрещиваем Бульдога с Носорогом

    // В начале делаем четкое разделение отвественности двух компонентов и далее
    // Объединяем Logger(Поведение) и Executor(Исполнение) в один Wrapper с именем LogService
    // через ШАБЛОННУЮ МАГИЮ
    // Теперь легко и непринужденно можно менять и Поведение и Исполнение
    // при помощи других разработанных классов.

    /*
    * Было вот так:
    ConsoleLogger logger;  // Реализация логгера
    SingleExecutor executor(logger);  // Однопоточный исполнитель (Executor)
    */
    /*
    Станет вот так
    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    Пошла уже Шаблонная магия
    */

int main() {

    // setlocale(LC_ALL, "RU");

    LogService<ConsoleLogger, SingleThreadExecutor> log_sevice;

    log_sevice.Start();
    /*
    log_sevice.Log(LogLevel::SUCCESS, "Приложение старт.");
    log_sevice.Log(LogLevel::INFO, "Это информационное сообщение.");
    log_sevice.Log(LogLevel::WARNING, "Что-то случилось...");
    log_sevice.Log(LogLevel::FATAL, "Sos. Спасите наши души!");
    */
    log_sevice.Log(LogLevel::SUCCESS, "Application started.");
    log_sevice.Log(LogLevel::INFO, "This is information message.");
    log_sevice.Log(LogLevel::WARNING, "It's seem to be something wrong...");
    log_sevice.Log(LogLevel::FATAL, "Fatal mtssage");

    std::this_thread::sleep_for(std::chrono::seconds(1));  //

    log_sevice.Stop();
    return 0;
}

