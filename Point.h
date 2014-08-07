#pragma once
#include "Board.h"

using namespace std;

namespace State
{
    enum Enum
    {
        Normal,
        Danger,
        Dead
    };
}

class Point
{
private:
    int x, y;
    State::Enum state; // Normal, Danger, Dead
    int sum; // ����(��ģ) �簢�� ������ ��
    int lastTurn; // �ֱٿ� �簢���� ���� ��
    Board* const board;

public:
    explicit Point(int x, int y, Board* board, State::Enum state = State::Normal);
    ~Point();

    // ��� ���� ���� ���� �Լ�
    inline bool IsDanger() { return state == State::Danger; }
    inline bool IsDead() { return state == State::Dead; }
    inline bool IsNormal() { return state == State::Normal; }
    inline int GetSum() { return sum; }
    inline void AddSum(const int number) { sum += number; }
    void ChangeState(State::Enum state);

    // ���� ĭ ���� �Լ�
    inline shared_ptr<Point> GetLeft() { return board->GetPoint(x - 1, y); }
    inline shared_ptr<Point> GetRight() { return board->GetPoint(x + 1, y); }
    inline shared_ptr<Point> GetUp() { return board->GetPoint(x, y - 1); }
    inline shared_ptr<Point> GetDown() { return board->GetPoint(x, y + 1); }
    inline shared_ptr<Point> GetRightDown() { return board->GetPoint(x + 1, y + 1); }

    bool PutSquare(const int number);

private:
    // �簢���� ���� �� �ִ� �ڸ����� Ȯ��
    bool CheckPutSquare();

    // ���� ĭ�� ���� ���¿� ���̴� ��Ȳ���� Ȯ�� �� ����
    void CheckAdjDead(shared_ptr<Point> checkPoint);
    // �ڽ��� ���� ���¿� ���̴� ��Ȳ���� Ȯ�� �� ����
    void CheckDanger();
    // �ڽ��� ���� ���¸� ������ �� �ִ� ��Ȳ���� Ȯ�� �� ����
    void CheckClearDanger();
};

