#include "AutoUser.h"
#include <iostream>

AutoUser::AutoUser(unique_ptr<Board> board)
: board(move(board)), maxSquareSum(0)
{
}


AutoUser::~AutoUser()
{
}


void AutoUser::AutoPlay()
{
    while (unputdownableSet.size() < Board::PutdownableSize)
    {
        int idx = rand() % Board::BufferSize;
        int x = rand() % (Board::XSize - 1);
        int y = rand() % (Board::YSize - 1);
        if (board->UseSquare(idx, x, y))
        {
            unputdownableSet.clear();
        }
        else
        {
            unputdownableSet.insert(UnputdownablePoint{ x, y });
        }
    }
    cout << board->GetDeadCnt() << endl;
    cout << "\t\t\t\t\t\t" << board->GetSquareSum() << endl;
    if (board->GetSquareSum() > maxSquareSum)
    {
        board->SaveToFile();
        maxSquareSum = board->GetSquareSum();
    }
}

void AutoUser::NewGame()
{
    board.release();
    board = make_unique<Board>();
    unputdownableSet.clear();
}