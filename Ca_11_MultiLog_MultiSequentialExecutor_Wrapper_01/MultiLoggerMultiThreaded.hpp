#pragma once

#include "MultiLogger.hpp"
#include "MultiSequentialExecutor.hpp"
#include "SingleExecutor.hpp"

// MultiLoggerMultiThreaded
template<typename Logger, typename Executor>
class MultiLoggerMultiThreaded {
private:
    Logger multiLogger; // ���������� ������ MultiLogger
    Executor multiExecutor; // ���������� MultiSequentialExecutor

public:
    // �����������
    MultiLoggerMultiThreaded(size_t numExecutors)
        : multiExecutor(numExecutors), multiLogger(multiExecutor) {
    }

    // ���������� �������
    void addLogger(const std::shared_ptr<ILogger>& logger) {
        multiLogger.addLogger(logger);
    }

    // �����������
    void log(MessageStatus status, const std::string& description) {
        multiLogger.log(status, description);
    }

    // ������ ���� �����
    void printAll() const {
        multiLogger.printAll();
    }
};


