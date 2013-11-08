#include "time.h"
#include "display.h"

#include <sys/time.h>

static const long NANOSECONDS_PER_SECOND = 1000000000L;

//static double g_freq;
static double g_delta;

void Time::Init()
{
//    LARGE_INTEGER li;
//    if(!QueryPerformanceFrequency(&li))
//		Display::Error("QueryPerformanceFrequency failed in timer initialization");
//    
//    g_freq = double(li.QuadPart);
    g_delta = 0;
}

void Time::SetDelta(double delta)
{
    g_delta = delta;
}

double Time::GetTime()
{
//    LARGE_INTEGER li;
//    if(!QueryPerformanceCounter(&li))
//		Display::Error("QueryPerformanceCounter failed in get time!");
//    
//    return double(li.QuadPart)/g_freq;

    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(((long) ts.tv_sec * NANOSECONDS_PER_SECOND) + ts.tv_nsec)/((double)(NANOSECONDS_PER_SECOND));
    
}

double Time::GetDelta()
{
    return g_delta;
}
