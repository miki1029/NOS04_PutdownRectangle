#pragma once
#include <memory>
#include <array>
#include <vector>
class Point;

using namespace std;

class Board
{
public:
    // ��� ����
    static const int BufferSize = 5;
    static const int InputSize = 16;
    static const int XSize = 9;
    static const int YSize = 9;
    static const int DeadCntMax = 81;
    static const int PutdownableSize = (Board::XSize - 1) * (Board::YSize - 1);

private:
    // Point
    array<array<shared_ptr<Point>, YSize>, XSize> arr; // ���� ������ Point�� ����
    const shared_ptr<Point> outPoint; // ���� ��� ������ �ϳ��� Point�� ó��
    int deadCnt; // ���� ĭ�� ��

    // Square
    array<int, InputSize> squareInput; // 1,2,3,...,9,8,...,2
    array<int, InputSize>::iterator curInput; // �簢�� input �ݺ���
    array<int, BufferSize> squareHand; // �簢�� �� ��� �ִ� 5��

    // Output
    struct Output { int n, x, y; };
    vector<Output> outputVect; // ������� �����ϴ� �ڷᱸ��

public:
    explicit Board();
    ~Board();

    // (x, y) ĭ Point�� ������
    inline shared_ptr<Point> GetPoint(int x, int y)
    {
        if (x < 0 || y < 0 || x > XSize - 1 || y > YSize - 1) return outPoint;
        else return arr[x][y];
    }
    // ���� ĭ �� ����
    inline void IncDeadCnt() { deadCnt++; }
    // ī�� ����� ������
    inline array<int, BufferSize>& GetSquareHand() { return squareHand; }
    // �� ��
    inline int GetTurn() { return outputVect.size() + 1; }
    // ĭ�� ��� �׾����� Ȯ��
    inline bool IsAllDead() { return deadCnt >= DeadCntMax; }
    inline int GetDeadCnt() { return deadCnt; }
    inline vector<Output> GetOutput() { return outputVect; }

    // (x, y)�� idx��° �簢���� �÷� ����
    bool UseSquare(int idx, int x, int y);

    void SaveToFile(const char* filename = "output.txt");

    int GetSquareSum();

private:
    // ���� �簢�� input�� ������
    int GetNextInput();
};