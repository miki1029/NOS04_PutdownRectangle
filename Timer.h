// 30�� Ÿ�̸Ӹ� ��Ÿ���� ������ Ŭ����
#pragma once
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

class Timer
{
private:
    thread th;
    bool runState;

public:
    Timer();
    ~Timer();

    inline bool RunState() { return runState; }

    void Start(int ms);
    void Stop();
};