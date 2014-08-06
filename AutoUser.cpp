#include "AutoUser.h"


AutoUser::AutoUser(Board* board)
: board(board)
{
}


AutoUser::~AutoUser()
{
}


void AutoUser::AutoPlay()
{
    for (int i = 0; i < 100; i++)
    {
        int idx = rand() % Board::BufferSize;
        int x = rand() % (Board::XSize - 1);
        int y = rand() % (Board::YSize - 1);
        board->UseSquare(idx, x, y);
    }
    board->SaveToFile();
}