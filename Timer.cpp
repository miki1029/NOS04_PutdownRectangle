#include "Timer.h"


Timer::Timer()
: runState(false)
{
}


Timer::~Timer()
{
    th.detach();
}

void Timer::Start(int ms)
{
    chrono::milliseconds time = chrono::milliseconds(ms);
    runState = true;

    th = thread([=]()
    {
        this_thread::sleep_for(time); // time 만큼 sleep
        runState = false; // false이면 실행 쓰레드들이 작업을 마침
        cout << "타이머 끝" << endl;
    });
}

void Timer::Stop()
{
    th.detach();
}