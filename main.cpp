#include <ctime>
#include "Timer.h"
#include "AutoUser.h"

Timer timer;

void RunThread(AutoUser* au)
{
    while (timer.RunState())
    {
        au->NewGame();
        au->AutoPlay();
    }
}

int main(void)
{
    /*unsigned int seed = (unsigned int)time(NULL);
    srand(seed);
    cout << seed << endl;*/

    timer.Start(590 * 1000);

    AutoUser au1(1);
    AutoUser au2(2);
    AutoUser au3(3);
    AutoUser au4(4);

    thread th1(&RunThread, &au1);
    thread th2(&RunThread, &au2);
    thread th3(&RunThread, &au3);
    thread th4(&RunThread, &au4);

    th1.join();
    th2.join();
    th3.join();
    th4.join();

    return 0;
}