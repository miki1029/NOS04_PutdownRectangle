#include <ctime>
#include "Timer.h"
#include "AutoUser.h"

int main(void)
{
    /*unsigned int seed = (unsigned int)time(NULL);
    srand(seed);
    cout << seed << endl;*/

    Timer timer;
    timer.Start(9 * 60 * 1000);
    AutoUser au;

    while (timer.RunState())
    {
        au.NewGame();
        au.AutoPlay();
    }

    return 0;
}