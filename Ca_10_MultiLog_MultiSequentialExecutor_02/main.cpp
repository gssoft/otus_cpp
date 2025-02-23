// Ca_10_MultiLog_MultiSequentialExecutor_02.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MultiSequentialExecutor.hpp"
#include "SingleExecutor.hpp"
#include "MultiLogger.hpp"
#include "RealLoggers.hpp"

int main() {
    // Создаем MultiSequentialExecutor с 3 SingleExecutor
    MultiSequentialExecutor<SingleExecutor> executor(3);

    // Создаем MultiLogger, используя MultiSequentialExecutor
    MultiLogger<MultiSequentialExecutor<SingleExecutor>> multiLogger(executor);

    //Добавляем логгеры
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->setup_channel_id(3);
    consoleLogger->enable(true);
    multiLogger.addLogger(consoleLogger);
    
     auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->setup_channel_id(2);
    memoryLogger->enable(true);
    multiLogger.addLogger(memoryLogger);
    
    
    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->setup_channel_id(1);
    fileLogger->set_file_name("logs.txt");
    fileLogger->enable(true);
    multiLogger.addLogger(fileLogger);
   
    multiLogger.log(MessageStatus::SUCCESS, "Message 1");
    multiLogger.log(MessageStatus::WARNING, "Message 2");
    multiLogger.log(MessageStatus::FATAL, "Message 3");
    multiLogger.log(MessageStatus::FATAL, "Message 4");
    multiLogger.log(MessageStatus::FATAL, "Message 5");
    multiLogger.log(MessageStatus::SUCCESS, "Message 6");
    multiLogger.log(MessageStatus::WARNING, "Message 7");
    multiLogger.log(MessageStatus::FATAL, "Message 8");
    multiLogger.log(MessageStatus::FATAL, "Message 9");
    multiLogger.log(MessageStatus::FATAL, "Message 10");
    
   std::this_thread::sleep_for(std::chrono::seconds(1));
    // Выводим все логи
    multiLogger.printAll();

    return 0;
}

