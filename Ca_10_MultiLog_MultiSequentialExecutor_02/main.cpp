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

/*
Программа представляет собой систему многоуровневого логирования с параллельным выполнением задач. Основные компоненты:

    MultiSequentialExecutor
        Управляет несколькими SingleExecutor для параллельного выполнения задач
        Распределяет задачи между исполнителями по ID
        Обеспечивает последовательное выполнение в каждом потоке
    SingleExecutor
        Последовательный исполнитель задач в отдельном потоке
        Использует std::jthread и std::stop_token
        Потокобезопасная очередь задач
    MultiLogger
        Менеджер логгеров с поддержкой нескольких каналов
        Использует MultiSequentialExecutor для параллельного выполнения
        Автоматическое присвоение ID каналам
    Логгеры
        ConsoleLogger - вывод в консоль
        MemoryLogger - хранение в памяти
        FileLogger - запись в файл
    DateTimeFormatter
        Форматирование времени в различных форматах
        Потокобезопасная реализация
        Поддержка микро- и миллисекунд

Код-ревью
1. MultiSequentialExecutor

    ✅ Хорошая структура класса и интерфейса
    ✅ Правильное использование unique_ptr
    ❌ Отсутствует обработка ошибок в конструкторе
    ❌ Отсутствие проверки на nullptr

2. SingleExecutor

    ✅ Современный подход с использованием std::jthread
    ✅ Правильная обработка stop_token
    ✅ Потокобезопасная реализация
    ❌ Сложная логика в методе work
    ❌ Отсутствие обработки исключений

3. MultiLogger

    ✅ Хорошая структура класса
    ✅ Правильное использование shared_ptr
    ✅ Автоматическое присвоение ID каналам
    ❌ Сложная логика в методе log
    ❌ Отсутствие обработки ошибок при создании LogItem

4. Логгеры

    ✅ Правильная реализация паттерна Strategy
    ✅ Корректное использование mutex
    ❌ Непоследовательное использование lock_guard
    ❌ Отсутствие обработки ошибок при работе с файлами
    ❌ Дублирование кода в реализации логгеров

5. DateTimeFormatter

    ✅ Хорошая структура кода и форматирование
    ✅ Правильное использование mutex
    ❌ Избыточные комментарии
    ❌ Отключенный код в комментариях
*/
/*
Общие замечания

    Потокобезопасность
        Некоторые методы не защищены mutex
    Обработка ошибок
        Отсутствие обработки ошибок при работе с файлами
        Нет обработки исключений в критических местах
    Кодирование
        Избыточные комментарии следует удалить
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
    Заменить магические числа на константы
*/