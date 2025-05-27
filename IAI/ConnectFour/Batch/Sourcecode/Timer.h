#ifndef TIMER_H_
#define TIMER_H_

#include <sys/time.h>
#include <cstddef>

const double timeLimit = 2.6e6; // 2.8 seconds in microseconds

class Timer
{
private:
    timeval start, now;

public:
    Timer();
    ~Timer();
    bool isTimeOut();
};

#endif