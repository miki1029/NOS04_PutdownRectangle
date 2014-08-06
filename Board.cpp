#include <iostream>
#include <fstream>
#include "Point.h"

Board::Board()
: outPoint(make_shared<Point>(-1, -1, this, State::Dead)), flagInc(false)
{
    // 2차원 Point 초기화
    for (int x = 0; x < XSize; x++)
    {
        for (int y = 0; y < YSize; y++)
        {
            State::Enum state = State::Normal;

            // 가장자리와 인접한 부분 위험 상태로
            if (x == 0 || x == XSize - 1) state = State::Danger;
            else if (y == 0 || y == YSize - 1) state = State::Danger;
            
            // 모서리 부분은 죽은 상태로
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

    // squareInput 초기화
    for (int i = 0; i < InputSize; i++)
    {
        squareInput[i] = i + 1;
    }
    curInput = squareInput.begin() + 1;

    // square 초기화
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
        // 결과 값 저장
        output.push_back(Output{ square[idx], x, y });
        // 다음 input 가져옴
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
    // 양 끝에 도달하면 방향을 반대로
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

    // 리턴 값 설정
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