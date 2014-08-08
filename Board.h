#pragma once
#include <memory>
#include <array>
#include <vector>
#include "defEnum.h"
class Point;

using namespace std;

class Board
{
public:
    // 상수 정의
    static const int BufferSize = 5;
    static const int InputSize = 16;
    static const int XSize = 9;
    static const int YSize = 9;
    static const int DeadCntMax = 81;
    static const int PutdownableSize = (Board::XSize - 1) * (Board::YSize - 1);

private:
    // Point
    array<array<shared_ptr<Point>, YSize>, XSize> arr; // 정상 범위의 Point를 저장
    const shared_ptr<Point> outPoint; // 판을 벗어난 영역을 하나의 Point로 처리
    int deadCnt; // 죽은 칸의 수

    // Square
    array<int, InputSize> squareInput; // 1,2,3,...,9,8,...,2
    array<int, InputSize>::iterator curInput; // 사각형 input 반복자
    array<int, BufferSize> squareHand; // 사각형 중 들고 있는 5개

    // Output
    struct Output { int n, x, y; };
    vector<Output> outputVect; // 결과값을 저장하는 자료구조

public:
    explicit Board();
    Board(const Board& arg);
    ~Board();

    // (x, y) 칸 Point를 가져옴
    inline shared_ptr<Point> GetPoint(int x, int y)
    {
        if (x < 0 || y < 0 || x > XSize - 1 || y > YSize - 1) return outPoint;
        else return arr[x][y];
    }
    inline void IncDeadCnt() { deadCnt++; } // 죽은 칸 수 증가
    inline array<int, BufferSize>& GetSquareHand() { return squareHand; } // 사각형 목록
    inline int GetTurn() { return outputVect.size() + 1; } // 턴 수
    inline bool IsAllDead() { return deadCnt >= DeadCntMax; } // 칸이 모두 죽었는지 확인
    inline int GetDeadCnt() { return deadCnt; } // 죽은 칸 수
    inline vector<Output> GetOutput() { return outputVect; } // 놓은 사각형 목록(결과값)
    int GetSquareSum();

    // (x, y)에 idx번째 사각형을 올려 놓음
    bool UseSquare(int idx, int x, int y);
    // 사각형을 가상으로 올려 상태 변화를 체크함
    ExpectPutdown::Enum CheckSquare(int idx, int x, int y,
        int* clearCnt, int* dangerCnt, int* deadCnt);

    void SaveToFile(const char* filename = "output.txt");

private:
    // 다음 사각형 input을 가져옴
    int GetNextInput();
};