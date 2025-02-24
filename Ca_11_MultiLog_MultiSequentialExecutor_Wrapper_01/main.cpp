// Ca_11_MultiLog_MultiSequentialExecutor_Wrapper_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MultiLoggerMultiThreaded.hpp"
#include "RealLoggers.hpp"

int main() {
    // Создаем MultiLoggerMultiThreaded с n SingleExecutor
    MultiLoggerMultiThreaded<                                   // Wrapper
        MultiLogger<MultiSequentialExecutor<SingleExecutor>>,   // MultiLogger
        MultiSequentialExecutor<SingleExecutor>>                // MultiSequentialExecutor
            multiLogger_multiThreaded_service(3);

    auto consoleLogger = std::make_shared<ConsoleLogger>();
    consoleLogger->setup_channel_id(3);
    consoleLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(consoleLogger);
    // multiLogger.addLogger(consoleLogger);

    auto memoryLogger = std::make_shared<MemoryLogger>();
    memoryLogger->setup_channel_id(2);
    memoryLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(memoryLogger);
   


    auto fileLogger = std::make_shared<FileLogger>();
    fileLogger->setup_channel_id(1);
    fileLogger->set_file_name("logs.txt");
    fileLogger->enable(true);
    multiLogger_multiThreaded_service.addLogger(fileLogger);

    // Логируем несколько сообщений
    multiLogger_multiThreaded_service.log(MessageStatus::SUCCESS, "Message 1");
    multiLogger_multiThreaded_service.log(MessageStatus::WARNING, "Message 2");
    multiLogger_multiThreaded_service.log(MessageStatus::FATAL, "Message 3");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Печатаем всё
    multiLogger_multiThreaded_service.printAll();

    return 0;
}
/*
Общее описание:

Проект представляет собой абстрактную систему логирования с использованием многопоточности и структуры "Исполнителей задач"
(Executors), куда входящие задачи распределяются по различным потокам.
Основная цель проекта — предоставить гибкий и расширяемый механизм логирования,
который может быть масштабирован для многопоточных приложений с использованием комбинации следующих компонентов:

    Логгеры (Loggers):
        Различные виды конкретных логгеров (например, ConsoleLogger, MemoryLogger, FileLogger),
        которые реализуют единый интерфейс ILogger.
    MultiLogger:
        Управляющий класс, работающий с несколькими логгерами. Отвечает за распределение задач логирования между ними.
    Executor'ы:
        Логирование производится асинхронно благодаря исполнителям задач:
            SingleExecutor: используется для выполнения задач последовательно в одном потоке.
            MultiSequentialExecutor: управляет группой SingleExecutor для поддержки многопоточности.
            MultiLoggerMultiThreaded: окончательная обертка, объединяющая MultiLogger с многопоточной поддержкой.
    Форматирование времени (DateTimeFormatter):
        Предоставляет различные форматы дат и времени, которые можно использовать для добавления временных меток в логи.

Основные компоненты:

    Логгеры:
        ConsoleLogger: выводит все логи в консоль.
        MemoryLogger: сохраняет логи в памяти до определенного лимита.
        FileLogger: сохраняет логи в файл.

Каждый логгер может быть включен/отключен, имеет возможность изменения формата времени
и поддерживает многопоточную синхронизацию.

    Структура LogItem:
        Используется для представления одной записи лога. Поля включают:
            Уникальный ID (id);
            ID канала (channel_id);
            Время записи (time);
            Статус сообщения (SUCCESS, WARNING, FATAL);
            Сообщение (description).
    MultiLogger:
        Работает с несколькими логгерами, передавая им задачи логирования через Executor.
    MultiLoggerMultiThreaded:
        Комбинирует MultiLogger с механизмом многопоточности, предоставленным MultiSequentialExecutor,
        для одновременного логирования в нескольких потоках.
    DateTimeFormatter:
        Используется для форматирования временных меток в сообщениях логов.

Потоковая архитектура (Executors):

Структура потокового исполнения представлена иерархией:

    SingleExecutor:
        Обрабатывает задачи по одной в своем собственном потоке.
    MultiSequentialExecutor:
        Хранит группу SingleExecutor'ов и распределяет задачи между ними.
    MultiLoggerMultiThreaded:
        Комбинирует MultiSequentialExecutor с MultiLogger для работы с несколькими логгерами в многопоточном режиме.

Особенности проекта:

    Многопоточность и балансировка моментов логирования:
        Все задачи логирования выполняются асинхронно через многопоточную архитектуру.
        Система использует распределение задач между исполнителями (executors), что уменьшает задержки в обработке.
    Модульность:
        Каждый компонент (логгер, executor и т.д.) существует отдельно, позволяя легко настроить или заменить один из модулей.
    Абстракция:
        Логгеры используют интерфейс (ILogger), что упрощает добавление новых типов логгеров.
    Форматирование времени:
        Поддержка нескольких форматов временных меток позволяет пользователю выбирать нужный формат.
    Защита данных:
        Использование std::mutex и std::lock_guard гарантирует безопасность при многопоточном доступе.

Код-ревью: сильные и слабые стороны
Cильные стороны:

    Многопоточность (использование std::mutex и std::jthread):
        Удачный выбор средств стандартной библиотеки для работы с многопоточностью.
        Использование std::stop_token для правильной остановки потоков.
    Обобщенность:
        MultiLogger и его взаимодействие с Executors обобщены, что делает систему легко расширяемой.
    Разделение обязанностей:
        Каждый класс отвечает за свои задачи (например, логгеры — за сбор логов, executors — за выполнение задач,
        DateTimeFormatter — за время).
        Это улучшает читаемость и тестируемость кода.
    Гибкость:
        Возможность добавления любых новых логгеров (например, для базы данных или сети) без изменения остальных частей кода.
    Форматирование времени:
        Аккуратное использование временных меток улучшает читаемость логов.
    Использование RAII:
        Автоматическое управление ресурсами (например, закрытие файла в FileLogger).

Слабые стороны/замечания:

    Использование дублирующих мьютексов:
        В некоторых местах используются "лишние" мьютексы (например, в MemoryLogger и FileLogger),
        что создает дополнительную нагрузку.
    Потенциальное "захлебывание потоков":
        При большом количестве задач логи могут обрабатываться в незамедлительном цикле без учета скорости их генерации.
        Это можно исправить добавлением механизма ограничения очередей.
    Ошибки обработки ошибок:
        Например, при открытии файла в FileLogger, если файл не удалось открыть,
        нужно выбрасывать исключение или выдавать более явное сообщение об ошибке.
    Дата-время и localtime_s:
        Функция localtime_s используется корректно, но более единообразное использование std::chrono
        позволит уменьшить зависимость от C-style функций времени.
    Конфигурация логгеров:
        Настройка channel_id вручную выглядит немного запутанной.
        Автоматическое управление channel_id с возможностью дополнительной индивидуальной настройки будет более удобным.
    Отсутствие тестирования:
        Код выглядит хорошо структурированным, но может потребоваться набор юнит-тестов для проверки.
    Управление состоянием (enabled):
        Включение/выключение логгеров (enable(bool)) можно сделать более надежным,
        используя атомарные переменные (std::atomic<bool>).

Рекомендации по улучшению:

    Добавить очередь задач с ограничением размера:
        Для MultiLogger добавить ограничение на максимальную длину очереди.
    Логгеры:
        Внедрить более явное управление channel_id, где ручная установка (setup_channel_id) будет опциональной.
    Снижение количества мьютексов:
        Некоторая оптимизация защиты данных снизит накладные расходы на синхронизацию.
    Обработка ошибок:
        Убедиться, что ошибки работы с файлом или временем корректно обрабатываются и логируются.
    Унификация работы с датой/временем:
        Отказаться от localtime_s в пользу std::chrono::zoned_time.
    Реализовать тесты:
        Написать автоматизированные тесты для компонентов (SingleExecutor, MultiLogger, логгеры).
    Документация методов:
        В коде отсутствует комментарии для методов. Добавьте их для лучшего понимания.

Итог

Код является очень хорошим примером продуманного и гибкого подхода к организации многопоточного логирования.
Тем не менее, упрощение настройки, оптимизация управления потоками и обработка ошибок сделают проект
более производительным и стабильным.
*/
