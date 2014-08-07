#include "AutoUser.h"
#include <iostream>
#include <string>

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

    int squareSum = board->GetSquareSum();
    if (squareSum > 1400)
    {
        string filename = to_string(squareSum) + ".txt";
        cout << filename << endl;
        board->SaveToFile(filename.c_str());
    }
    if (squareSum > maxSquareSum)
    {
        board->SaveToFile();
        maxSquareSum = squareSum;
        cout << maxSquareSum << endl;
    }
}

void AutoUser::NewGame()
{
    board = make_unique<Board>();
    unputdownableSet.clear();
}