#include "DateTimeFormatter.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

#include <string>



// ������� ��� ��������� �������� ������� � �������� �������
std::string formatDateTime(const DateTimeFormat& format);

std::string formatDateTime(const DateTimeFormat& format) {
    using namespace std::chrono;

    // �������� ������� ��������� �����
    auto now = system_clock::now();
    auto now_time_t = system_clock::to_time_t(now);
    auto now_us = duration_cast<microseconds>(now.time_since_epoch()) % 1000000;
    auto now_ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    /*
    // ������������ ��������� ����� � ���������
    std::tm local_tm = *std::localtime(&now_time_t);
    */
    // ������������ ��������� ����� � ���������
    std::tm local_tm;
    errno_t err = localtime_s(&local_tm, &now_time_t);

    if (err != 0) {
        throw std::runtime_error("������ ��� ����������� �������: localtime_s failed");
    }

    // OR
    /*
    try {
        std::tm local_tm;
        errno_t err = localtime_s(&local_tm, &now_time_t);
    }
    catch (const std::exception& e) {
        // ��������� ������ �� ����� ������� ������
        logger.error(e.what());
        return false;
    }
    */
    // ����� ��� �������������� ������ �������
    std::ostringstream oss;

    // ���������� ������ ������� � ����������� �� ����������� �������� `format`
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
