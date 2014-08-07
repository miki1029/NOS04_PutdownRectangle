#include <iostream>
#include <ctime>
#include "Timer.h"
#include "AutoUser.h"

int main(void)
{

    srand((unsigned int)time(NULL));

    Timer timer;
    timer.Start(9 * 60 * 1000);
    AutoUser au;

    while (timer.RunState())
    {
        cout << "½ÃÀÛ" << endl;
        au.NewGame();
        au.AutoPlay();
    }

    return 0;
}