#ifndef TIMER_HPP
#define TIMER_HPP

#include <sys/time.h>
class Timer
{
public:
  Timer()
  {
    start.tv_sec = start.tv_usec = 0;
    gettimeofday(&start, NULL);
  }

  double getElapsedMicroseconds()
  {
    gettimeofday(&end, NULL);
    return (end.tv_sec - start.tv_sec) * 1e6 + end.tv_usec - start.tv_usec;
  }

protected:
  timeval start, end;
};

#endif
