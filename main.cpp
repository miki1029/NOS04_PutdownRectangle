#include "AutoUser.h"
#include <ctime>
#include <iostream>

int main(void)
{
    srand((unsigned int)time(NULL));

    AutoUser au(new Board());

    cout << "½ÃÀÛ" << endl;
    au.AutoPlay();

    return 0;
}