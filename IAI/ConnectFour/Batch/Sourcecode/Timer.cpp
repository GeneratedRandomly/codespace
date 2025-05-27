#include "Timer.h"

Timer::Timer() : start{0, 0}
{
    gettimeofday(&start, NULL);
}

Timer::~Timer() {}

bool Timer::isTimeOut()
{
    gettimeofday(&now, NULL);
    double elapsed = (now.tv_sec - start.tv_sec) * 1e6 + now.tv_usec - start.tv_usec;
    return elapsed > timeLimit;
}