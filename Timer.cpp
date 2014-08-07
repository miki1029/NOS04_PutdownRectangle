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
        this_thread::sleep_for(time); // time ��ŭ sleep
        runState = false; // false�̸� ���� ��������� �۾��� ��ħ
        cout << "Ÿ�̸� ��" << endl;
    });
}

void Timer::Stop()
{
    th.detach();
}