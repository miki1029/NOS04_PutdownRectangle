#pragma once
#include <set>
#include "Board.h"

class AutoUser
{
private:
    unique_ptr<Board> board;
    int maxSquareSum;

    struct UnputdownablePoint { 
        int x, y;
        inline bool operator==(const UnputdownablePoint& arg) const
        {
            return x == arg.x && y == arg.y;
        }
        inline bool operator!=(const UnputdownablePoint& arg) const
        {
            return x != arg.x || y != arg.y;
        }
        inline bool operator>(const UnputdownablePoint& arg) const
        {
            return x == arg.x ? y > arg.y : x > arg.x;
        }
        inline bool operator<(const UnputdownablePoint& arg) const
        {
            return x == arg.x ? y < arg.y : x < arg.x;
        }
        inline bool operator>=(const UnputdownablePoint& arg) const
        {
            return x == arg.x ? y >= arg.y : x >= arg.x;
        }
        inline bool operator<=(const UnputdownablePoint& arg) const
        {
            return x == arg.x ? y <= arg.y : x <= arg.x;
        }
    };
    set<UnputdownablePoint> unputdownableSet;

public:
    AutoUser(unique_ptr<Board> board = nullptr);
    ~AutoUser();

    void AutoPlay();
    void NewGame();
};

