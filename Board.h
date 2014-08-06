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
    static const int InputSize = 9;
    static const int XSize = 9;
    static const int YSize = 9;

private:

    array<array<shared_ptr<Point>, YSize>, XSize> arr; // ���� ������ Point�� ����
    const shared_ptr<Point> outPoint; // ���� ��� ������ �ϳ��� Point�� ó��

    array<int, InputSize> squareInput; // 1,2,3,...,9
    array<int, BufferSize> square; // ī�� �� ��� �ִ� 5��

    array<int, InputSize>::iterator curInput; // ī�帮��Ʈ �ݺ���
    bool flagInc; // �ݺ��� flag. true: ����, false: ����

    struct Output { int n, x, y; };
    vector<Output> output;

public:
    explicit Board();
    ~Board();

    // (x, y) ĭ Point�� ������
    inline shared_ptr<Point> GetPoint(int x, int y)
    {
        if (x < 0 || y < 0 || x > XSize - 1 || y > YSize - 1) return outPoint;
        else return arr[x][y];
    }
    // ī�� ����� ������
    inline array<int, InputSize>& GetSquareInput() { return squareInput; }

    // (x, y)�� idx��° �簢���� �÷� ����
    void UseSquare(int idx, int x, int y);

    void SaveToFile();

private:
    // ���� �簢�� input�� ������
    int GetNextInput();
};