#include "LogService.hpp"

#include <mutex>

// Реализация функции форматирования времени
std::string formatDateTime(const DateTimeFormat& format) {
    using namespace std::chrono;


    // Получаем текущее системное время
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    auto now_us = duration_cast<microseconds>(now.time_since_epoch()) % 1000000;
    auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    // Конвертируем системное время в локальное
    std::tm local_tm;
    errno_t err = localtime_s(&local_tm, &now_time_t);

    if (err != 0) {
        throw std::runtime_error("Ошибка при конвертации времени: localtime_s failed");
    }

    // Поток для форматирования строки времени
    std::ostringstream oss;

    // Определяем формат времени в зависимости от переданного значения `format`
    switch (format) {
    case DateTimeFormat::WithDate_HH_MM:
        oss << std::put_time(&local_tm, "%y.%m.%d %H:%M");
        break;

    case DateTimeFormat::WithDate_HH_MM_SS:
        oss << std::put_time(&local_tm, "%y.%m.%d %H:%M:%S");
        break;

    case DateTimeFormat::WithDate_HH_MM_SS_ms:
        oss << std::put_time(&local_tm, "%y.%m.%d %H:%M:%S") << '.'
            << std::setfill('0') << std::setw(3) << now_ms.count();
        break;

    case DateTimeFormat::WithDate_HH_MM_SS_us:
        oss << std::put_time(&local_tm, "%y.%m.%d %H:%M:%S") << '.'
            << std::setfill('0') << std::setw(6) << now_us.count();
        break;

    case DateTimeFormat::WithoutDate_HH_MM:
        oss << std::put_time(&local_tm, "%H:%M");
        break;

    case DateTimeFormat::WithoutDate_HH_MM_SS:
        oss << std::put_time(&local_tm, "%H:%M:%S");
        break;

    case DateTimeFormat::WithoutDate_HH_MM_SS_ms:
        oss << std::put_time(&local_tm, "%H:%M:%S") << '.'
            << std::setfill('0') << std::setw(3) << now_ms.count();
        break;

    case DateTimeFormat::WithoutDate_HH_MM_SS_us:
        oss << std::put_time(&local_tm, "%H:%M:%S") << '.'
            << std::setfill('0') << std::setw(6) << now_us.count();
        break;
    }

    return oss.str();
}


std::mutex log_string_mutex;

// Реализация метода Log для ConsoleLogger
void ConsoleLogger::Log(const LogItem& item) {

    {
        const std::lock_guard<std::mutex> lock(log_string_mutex);

        std::string timeString = formatDateTime(item.dateTimeFormat);

        switch (item.option) {
        case LogLevel::SUCCESS:
            std::cout << "[" << timeString << "] [SUCCESS] ";
            break;
        case LogLevel::WARNING:
            std::cout << "[" << timeString << "] [WARNING] ";
            break;
        case LogLevel::FATAL:
            std::cout << "[" << timeString << "] [FATAL  ] ";
            break;
        case LogLevel::INFO:
            std::cout << "[" << timeString << "] [INFO   ] ";
            break;
        case LogLevel::ERROR:
            std::cout << "[" << timeString << "] [ERROR  ] ";
            break;
        case LogLevel::DEBUG:
            std::cout << "[" << timeString << "] [DEBUG  ] ";
            break;
        case LogLevel::TRACE:
            std::cout << "[" << timeString << "] [TRACE  ] ";
            break;
        case LogLevel::CRITICAL:
            std::cout << "[" << timeString << "] [CRITICAL] ";
            break;
        case LogLevel::EXCEPTION:
            std::cout << "[" << timeString << "] [EXCEPTION] ";
            break;
        }

        std::cout << item.param1;
        if (!item.param2.empty()) {
            std::cout << " " << item.param2;
        }
        if (!item.param3.empty()) {
            std::cout << " " << item.param3;
        }
        std::cout << std::endl;
    }
}
