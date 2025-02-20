#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>
#include <iostream>

// Перечисление доступных форматов времени
enum class DateTimeFormat {
    WithDate_HH_MM,              // yy.mm.dd hh:mm
    WithDate_HH_MM_SS,           // yy.mm.dd hh:mm:ss
    WithDate_HH_MM_SS_ms,        // yy.mm.dd hh:mm:ss.mmm
    WithDate_HH_MM_SS_us,        // yy.mm.dd hh:mm:ss.uuuuuu
    WithoutDate_HH_MM,           // hh:mm
    WithoutDate_HH_MM_SS,        // hh:mm:ss
    WithoutDate_HH_MM_SS_ms,     // hh:mm:ss.mmm
    WithoutDate_HH_MM_SS_us,     // hh:mm:ss.uuuuuu
};

// Функция для получения текущего времени в заданном формате
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


