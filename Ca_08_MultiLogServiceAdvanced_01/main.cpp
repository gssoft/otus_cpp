// Ca_08_MultiLogServiceAdvanced_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "MultiLogServiceAdvanced.hpp"
#include "MultiSequentialExecutor.hpp"

/*
    В предыдущем проекте мы остановились вот на этом
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;

    Но вдруг сюда неожиданно примчались всезнающие и особоодаренные люди (они очень долго на ассемблере писали)
    и сказали, что все НЕПРАЛЬНА.
    Все логеры - разные и имеют большую разницу в скорости логирования одной лог-записи.
    Медленные Loggers (FileLogger и ConsoleLogger) будут тормозить быстрых.
    Тем более, что надо еще DataBaseLogger, HttpLogger, GrpcLogger сделать - срочно, еще вчера.
    И еще надо чтобы последовательный порядок лог-записей был.
    Поэтому нам надо однопоточный <SingleThreadExecutor>  заменить на многоканальный MultiSequentialExecutor.
    Придется сделать
    Реализация ниже. Только благодаря нашей шаблонной магии мы меняем ссответсвующий компонент.
    Мы изменили наименование wrapper c MultiLogService на MultiLogServiceAdvanced
    и реализовали новый Executor: MultiSequentialExecutor - Многоканальный Последовательный Executor.
    Каждому Логгеру выделяем свой собственный последовательный канал длл обособленного исплнения и
    для сохранения последовательного порядка лог-записей.

    Создаем MultiLogServiceAdvanced с многопоточным последовательным исполнителем
    Было так:
    MultiLogService<MultiLogger, SingleThreadExecutor> multi_log_service;
    Стало так:
    MultiLogServiceAdvanced<MultiLogger, MultiSequentialExecutor> multi_log_service_adv(3); // 3 потока.
*/

int main() {

    MultiLogServiceAdvanced<MultiLogger, MultiSequentialExecutor> multi_log_service_adv(3); // 3 потока. 

    // Добавляем ConsoleLogger
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    // consoleLogger->enable(false);
    consoleLogger->setup_channel_id(1); // Канал 1
    multi_log_service_adv.addLogger(consoleLogger);

    // Добавляем MemoryLogger
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    // memoryLogger->enable(false);
    memoryLogger->setup_channel_id(2); // Канал 2
    multi_log_service_adv.addLogger(memoryLogger);

    // Добавляем FileLogger
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->set_file_name("FileLogger.txt");
    fileLogger->enable(true);
    // fileLogger->enable(false);
    fileLogger->setup_channel_id(3); // Канал 3
    multi_log_service_adv.addLogger(fileLogger);

    // Логируем несколько сообщений

    multi_log_service_adv.log(1, MessageStatus::SUCCESS, "Message1 on channel 1");
    multi_log_service_adv.log(1, MessageStatus::WARNING, "Message2 on channel 2");
    multi_log_service_adv.log(1, MessageStatus::FATAL, "Message3 on channel 3");

    // Печатаем все логи
    multi_log_service_adv.printAll();

    // Показываем состояние исполнителя
    // to Do
  //   std::cout << "Active threads: " << multi_log_service_adv.getActiveThreadCount() << "\n";
  //   std::cout << "Pending tasks: " << multi_log_service_adv.getTaskQueueSize() << "\n";

    return 0;
}

/*
    Таким образом мы прошли путь от самого примитивного ConsoleLogger до
    MultoLogService, работающего в последовательном мультиканальном ThreadPool.
*/

/*
Программа работает.
Требует дополнительной отладки
*/

