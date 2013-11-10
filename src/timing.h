#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

namespace Time
{
    void Init();
    double GetTime();
    
    void SetDelta(double delta);
    double GetDelta();
};

#endif // TIMING_H_INCLUDED
