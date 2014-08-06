#pragma once
#include <memory>
#include <array>
#include <vector>
class Point;

using namespace std;

class Board
{
public:
    // 상수 정의
    static const int BufferSize = 5;
    static const int InputSize = 9;
    static const int XSize = 9;
    static const int YSize = 9;

private:

    array<array<shared_ptr<Point>, YSize>, XSize> arr; // 정상 범위의 Point를 저장
    const shared_ptr<Point> outPoint; // 판을 벗어난 영역을 하나의 Point로 처리

    array<int, InputSize> squareInput; // 1,2,3,...,9
    array<int, BufferSize> square; // 카드 중 들고 있는 5개

    array<int, InputSize>::iterator curInput; // 카드리스트 반복자
    bool flagInc; // 반복자 flag. true: 증가, false: 감소

    struct Output { int n, x, y; };
    vector<Output> output;

public:
    explicit Board();
    ~Board();

    // (x, y) 칸 Point를 가져옴
    inline shared_ptr<Point> GetPoint(int x, int y)
    {
        if (x < 0 || y < 0 || x > XSize - 1 || y > YSize - 1) return outPoint;
        else return arr[x][y];
    }
    // 카드 목록을 가져옴
    inline array<int, InputSize>& GetSquareInput() { return squareInput; }

    // (x, y)에 idx번째 사각형을 올려 놓음
    void UseSquare(int idx, int x, int y);

    void SaveToFile();

private:
    // 다음 사각형 input을 가져옴
    int GetNextInput();
};