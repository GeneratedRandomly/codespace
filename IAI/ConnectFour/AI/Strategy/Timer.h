#include <sys/time.h>
#include <cstddef>

const double timeLimit = 2.8e6; // 2.5 seconds in microseconds

class Timer
{
private:
    timeval start, now;

public:
    Timer();
    ~Timer();
    bool isTimeOut();
};
Timer::Timer() : start{0, 0} { gettimeofday(&start, NULL); }

Timer::~Timer() {}

bool Timer::isTimeOut()
{
    gettimeofday(&now, NULL);
    double elapsed = (now.tv_sec - start.tv_sec) * 1e6 + now.tv_usec - start.tv_usec;
    return elapsed > timeLimit;
}