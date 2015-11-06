#pragma once

#include <time.h>

class CHgzTimer
{
public:
    CHgzTimer(void)  { _time64(&m_time); };
    ~CHgzTimer(void) {}

    __time64_t Start(void)   { _time64(&m_time); return m_time; }
    __time64_t GetSpan(void) { __time64_t tim = 0; _time64(&tim); return tim-m_time; }
    __time64_t GetTime(void) { __time64_t tim = 0; _time64(&tim); return tim; }

private:
    __time64_t m_time;
    
};

