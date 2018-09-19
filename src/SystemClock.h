// This file is a part of std-adiog project.
// Copyright 2018 Aleksander Gajewski <adiog@brainfuck.pl>.

#pragma once

#include <chrono>
#include <cstdint>


struct SystemClock
{
public:
    static uint64_t millis()
    {
        std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
        uint64_t millisSinceEpoch =
            std::chrono::duration_cast<std::chrono::milliseconds>(timer.time_since_epoch()).count();
        return millisSinceEpoch;
    }

    static uint64_t micros()
    {
        std::chrono::time_point<std::chrono::system_clock> timer = std::chrono::system_clock::now();
        uint64_t microsSinceEpoch =
            std::chrono::duration_cast<std::chrono::microseconds>(timer.time_since_epoch()).count();
        return microsSinceEpoch;
    }
};

