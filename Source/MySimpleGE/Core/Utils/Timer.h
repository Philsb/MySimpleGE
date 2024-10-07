#pragma once
#include <chrono>
#include <iostream>

namespace MSGE
{
//Credit goes to https://www.youtube.com/watch?v=YG4jexlSAjc
class ScopedTimer
{
public:
    ScopedTimer()
    {
        _startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ScopedTimer(const std::string& customOutput )
    : _customOutput(customOutput)
    {
        _startTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~ScopedTimer()
    {
    }

    void stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(_startTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = end - start; 
        double ms = duration * 0.001;
        std::cout << _customOutput << duration << "us (" << ms << "ms)\n";
    }
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _startTimepoint;
    std::string _customOutput;
};
} // namespace MSGE


