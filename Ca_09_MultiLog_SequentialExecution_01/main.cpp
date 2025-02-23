// Ca_09_MultiLog_SequentialExecution_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MultiLogger.hpp"
#include "RealLoggers.hpp"
#include "SingleExecutor.hpp"

int main() {
    // Создаем SingleExecutor
    SingleExecutor executor;

    // Создаем MultiLogger, параметризованный SingleExecutor
    MultiLogger<SingleExecutor> multiLogger(executor);

    // Добавляем консольный логгер
    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->enable(true);
    consoleLogger->setup_channel_id(1);
    multiLogger.addLogger(consoleLogger);

    // Добавляем логгер в память
    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->enable(true);
    memoryLogger->setup_channel_id(2);
    multiLogger.addLogger(memoryLogger);

    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->setup_channel_id(3);
    fileLogger->set_file_name("logs.txt");
    fileLogger->enable(true);
    multiLogger.addLogger(fileLogger);

    // Отправляем несколько сообщений в лог
    multiLogger.log(MessageStatus::SUCCESS, "Initialization complete");
    multiLogger.log(MessageStatus::WARNING, "Low disk space");
    multiLogger.log(MessageStatus::FATAL, "Unexpected error occurred");

    multiLogger.log(MessageStatus::SUCCESS, "Initialization complete");
    multiLogger.log(MessageStatus::WARNING, "Low disk space");
    multiLogger.log(MessageStatus::FATAL, "Unexpected error occurred");

    // Задержка, чтобы дать потоку время обработать задания
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Выводим все логи (память)
    std::cout << "Logs from MemoryLogger: " << std::endl;
    memoryLogger->print();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    return 0;
}

/*
Программа представляет собой систему многоуровневого логирования с последовательным выполнением задач. Основные компоненты:

    DateTimeFormatter
        Форматирование времени в различных форматах (с датой и без)
        Поддержка микро- и миллисекунд
        Потокобезопасная реализация с использованием mutex
    MultiLogger
        Менеджер логгеров с поддержкой нескольких каналов
        Последовательное выполнение через SingleExecutor
        Управление несколькими логгерами одновременно
    Логгеры
        ConsoleLogger - вывод в консоль
        MemoryLogger - хранение в памяти (ограниченный размер)
        FileLogger - запись в файл
    SingleExecutor
        Последовательный исполнитель задач
        Использование C++20 std::jthread
        Безопасное завершение через std::stop_token

Код-ревью
1. DateTimeFormatter

    ✅ Хорошая структура кода и форматирование
    ✅ Правильное использование mutex для потокобезопасности
    ❌ Избыточные комментарии (например, "Получаем текущее системное время")
    ❌ Отключенный код в комментариях (localtime_s)

2. MultiLogger

    ✅ Хорошая структура классов и интерфейсов
    ✅ Правильное использование smart pointers
    ❌ Слишком длинные методы (например, log в MultiLogger)
    ❌ Отсутствие обработки ошибок при работе с файлами

3. Логгеры

    ✅ Правильная реализация паттерна Strategy
    ✅ Корректное использование mutex
    ❌ Непоследовательное использование lock_guard
    ❌ Отсутствие обработки ошибок при работе с файлами

4. SingleExecutor

    ✅ Современный подход с использованием std::jthread
    ✅ Правильная обработка завершения через stop_token
    ❌ Сложная логика в методе work
    ❌ Отсутствие обработки исключений
*/
/*
Общие замечания

    Потокобезопасность
        Некоторые методы не защищены mutex
    Обработка ошибок
        Отсутствие обработки ошибок при работе с файлами
        Нет обработки исключений в критических местах
    Кодирование
        Некоторые методы слишком длинные
    Архитектура
        Хорошая структура классов и интерфейсов
        Правильное использование паттернов
        Можно извлечь общую логику в базовый класс

Рекомендуется:

    Создать базовый класс для логгеров
    Добавить обработку ошибок
    Унифицировать использование mutex
    Разделить длинные методы на более мелкие
*/

