#include "AutoUser.h"
#include <ctime>
#include <iostream>

int main(void)
{
    srand((unsigned int)time(NULL));

    AutoUser au;

    while (true)
    {
        cout << "����" << endl;
        au.NewGame();
        au.AutoPlay();
    }

    return 0;
}