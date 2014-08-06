#include <iostream>
#include <fstream>
#include "Point.h"

Board::Board()
: outPoint(make_shared<Point>(-1, -1, this, State::Dead)), flagInc(false)
{
    // 2���� Point �ʱ�ȭ
    for (int x = 0; x < XSize; x++)
    {
        for (int y = 0; y < YSize; y++)
        {
            State::Enum state = State::Normal;

            // �����ڸ��� ������ �κ� ���� ���·�
            if (x == 0 || x == XSize - 1) state = State::Danger;
            else if (y == 0 || y == YSize - 1) state = State::Danger;
            
            // �𼭸� �κ��� ���� ���·�
            if ((x == 0 && y == 0) ||
                (x == 0 && y == YSize - 1) ||
                (x == XSize - 1 && y == 0) ||
                (x == XSize - 1 && y == YSize - 1))
            {
                state = State::Dead;
            }

            arr[x][y] = make_shared<Point>(x, y, this, state);
        }
    }

    // squareInput �ʱ�ȭ
    for (int i = 0; i < InputSize; i++)
    {
        squareInput[i] = i + 1;
    }
    curInput = squareInput.begin() + 1;

    // square �ʱ�ȭ
    for (int i = 0; i < BufferSize; i++)
    {
        square[i] = GetNextInput();
    }
}


Board::~Board()
{
}


void Board::UseSquare(int idx, int x, int y)
{
    if (GetPoint(x, y)->PutSquare(square[idx]))
    {
        // ��� �� ����
        output.push_back(Output{ square[idx], x, y });
        // ���� input ������
        square[idx] = GetNextInput();
    }
}

void Board::SaveToFile()
{
    ofstream outStream;
    outStream.open("output.txt");

    for each (Output itr in output)
    {
        outStream << itr.n << " " << itr.x << " " << itr.y << endl;
    }

    outStream.close();
}

int Board::GetNextInput()
{
    // �� ���� �����ϸ� ������ �ݴ��
    if (curInput == squareInput.end())
    {
        flagInc = false;
        curInput --;
    }
    else if (curInput == squareInput.begin())
    {
        flagInc = true;
        curInput++;
    }

    // ���� �� ����
    int next = 0;
    if (flagInc)
    {
        next = *curInput;
        curInput++;
    }
    else
    {
        curInput--;
        next = *curInput;
    }

    return next;
}