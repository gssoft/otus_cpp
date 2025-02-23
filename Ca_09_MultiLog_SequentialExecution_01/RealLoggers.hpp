#pragma once

#ifndef REALLOGGERS_HPP
#define REALLOGGERS_HPP

#include "DateTimeFormatter.hpp"
#include "MultiLogger.hpp"
#include <fstream>
#include <mutex>

class ConsoleLogger : public ILogger {
private:
    bool enabled;
    DateTimeFormat dtFormat = DateTimeFormat::WithDate_HH_MM_SS;
    std::mutex _mutex;
public:
    ConsoleLogger() : enabled(false) {}

    void enable(bool status) override {
        enabled = status;
    }

    void log(const LogItem& item) override {
        // const std::lock_guard<std::mutex> lock(_mutex);
        if (enabled) {
            std::cout << formatDateTime(dtFormat) << ' ' << item.toString() << std::endl;
        }
    }

    void print() const override {
        // std::cout << "ConsoleLogger: logs are printed directly in real-time." << std::endl;
    }

    bool isEnabled() const override {
        return enabled;
    }

    void set_dt_format(DateTimeFormat format) override {
        dtFormat = format;
    }
};

class MemoryLogger : public ILogger {
private:
    bool enabled;
    std::vector<LogItem> logs;
    size_t maxLogItems = 100;
    DateTimeFormat dtFormat = DateTimeFormat::WithDate_HH_MM_SS;
    const std::mutex _mutex;
    std::mutex _mutex_;

public:
    MemoryLogger() : enabled(false) {}

    void enable(bool status) override {
        enabled = status;
    }

    void log(const LogItem& item) override {
        // const std::lock_guard<std::mutex> lock(_mutex_);
        if (enabled && logs.size() >= maxLogItems) {
            logs.erase(logs.begin());
        }
        if (enabled) {
            logs.push_back(item);
        }
    }

    void print() const override {
        // const std::lock_guard<std::mutex> lock(_mutex);
        std::cout << "MemoryLogger stored logs:" << std::endl;
        for (const auto& log : logs) {
            std::cout << formatDateTime(dtFormat) << ' ' << log.toString() << std::endl;
        }
    }

    bool isEnabled() const override {
        return enabled;
    }

    void set_max_logitems(size_t max_items) {
        maxLogItems = max_items;
    }

    void set_dt_format(DateTimeFormat format) override {
        dtFormat = format;
    }
};

class FileLogger : public ILogger {
private:
    bool enabled;
    std::ofstream logFile;
    std::string filename;
    DateTimeFormat dtFormat = DateTimeFormat::WithDate_HH_MM_SS;
    std::mutex _mutex;
    std::mutex __mutex;
public:
    FileLogger() : enabled(false), logFile() {}

    void set_file_name(const std::string& fname) {
        filename = fname;
    }

    void open_file() {
        if (!filename.empty()) {
            logFile.open(filename, std::ios::app); // Открываем файл в режиме добавления
        }
    }

    void close_file() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void enable(bool status) override {
        enabled = status;
        if (enabled) {
            open_file();
        }
        else {
            close_file();
        }
    }

    void log(const LogItem& item) override {
        const std::lock_guard<std::mutex> lock(_mutex);
        if (enabled && logFile.is_open()) {
            logFile << formatDateTime(dtFormat) << ' ' << item.toString() << std::endl;
        }
    }

    void print() const override {

        // std::lock_guard<std::mutex> lock(__mutex);
        std::ifstream infile(filename);
        if (infile.is_open()) {
            std::cout << "FileLogger contents:\n";
            std::string line;
            while (getline(infile, line)) {
                std::cout << line << '\n';
            }
            infile.close();
        }
        else {
            std::cerr << "Error opening file: " << filename << std::endl;
        }

    }

    bool isEnabled() const override {
        return enabled;
    }

    void set_dt_format(DateTimeFormat format) override {
        dtFormat = format;
    }
};

#endif // REALLOGGERS_HPP


