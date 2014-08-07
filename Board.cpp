#include <fstream>
#include "Point.h"

Board::Board()
: outPoint(make_shared<Point>(-1, -1, this, State::Dead)), deadCnt(4)
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
    for (int i = 0; i < 9; i++)
    {
        squareInput[i] = i + 1;
    }
    for (int i = 9; i < InputSize; i++)
    {
        squareInput[i] = 17 - i;
    }
    curInput = squareInput.begin();

    // square �ʱ�ȭ
    for (int i = 0; i < BufferSize; i++)
    {
        squareHand[i] = GetNextInput();
    }
}


Board::~Board()
{
}


bool Board::UseSquare(int idx, int x, int y)
{
    if (GetPoint(x, y)->PutSquare(squareHand[idx]))
    {
        // ��� �� ����
        outputVect.push_back(Output{ squareHand[idx], x, y });
        // ���� input ������
        squareHand[idx] = GetNextInput();

        return true;
    }
    else return false;
}

void Board::SaveToFile(const char* filename)
{
    ofstream outStream;
    outStream.open(filename);

    outStream << GetSquareSum() << endl;
    for each (Output output in outputVect)
    {
        outStream << output.n << " " << output.x << " " << output.y << endl;
    }

    outStream.close();
}

int Board::GetNextInput()
{
    // ���� �����ϸ� ó������
    if (curInput == squareInput.end())
    {
        curInput = squareInput.begin();
    }

    return *(curInput++);
}

int Board::GetSquareSum()
{
    int sum = 0;
    for each (Output output in outputVect)
    {
        sum += output.n;
    }
    return sum;
}