#ifndef MATRIX_MEASUREEXECUTION_H
#define MATRIX_MEASUREEXECUTION_H


#include "time_measurement.h"

#include <iostream>
#include <string>
#include <utility>

// RAII wrapper for time measurement
class MeasureExecution {
public:
    explicit MeasureExecution(std::string message) : _message(std::move(message)),
        _start(get_current_time_fenced()) {}

    ~MeasureExecution()
    {
        auto finish = get_current_time_fenced();
        auto totalTime = finish - _start;
        std::cout << _message << to_us(totalTime) << std::endl;
    }

private:
    std::string _message{};
    std::chrono::high_resolution_clock::time_point _start{};
};


#endif //MATRIX_MEASUREEXECUTION_H
