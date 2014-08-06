#pragma once
#include "Board.h"

class AutoUser
{
private:
    Board* board;
public:
    AutoUser(Board* board);
    ~AutoUser();

    void AutoPlay();
};

