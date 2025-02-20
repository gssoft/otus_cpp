#pragma once
#pragma once

#ifndef DATETIMEFORMATTER_HPP
#define DATETIMEFORMATTER_HPP

#include <string>

// Перечисление доступных форматов времени
enum class DateTimeFormat {
    WithDate_HH_MM,              // yy.mm.dd hh:mm
    WithDate_HH_MM_SS,           // yy.mm.dd hh:mm:ss
    WithDate_HH_MM_SS_ms,        // yy.mm.dd hh:mm:ss.mmm
    WithDate_HH_MM_SS_us,        // yy.mm.dd hh:mm:ss.uuuuuu
    WithoutDate_HH_MM,           // hh:mm
    WithoutDate_HH_MM_SS,        // hh:mm:ss
    WithoutDate_HH_MM_SS_ms,     // hh:mm:ss.mmm
    WithoutDate_HH_MM_SS_us      // hh:mm:ss.uuuuuu
};

// Функция для получения текущего времени в заданном формате
std::string formatDateTime(const DateTimeFormat& format);

#endif // DATETIMEFORMATTER_HPP


