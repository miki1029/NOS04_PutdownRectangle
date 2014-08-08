#pragma once
#include "defEnum.h"
#include "Board.h"

using namespace std;

class Point
{
private:
    int x, y;
    State::Enum state; // Normal, Danger, Dead
    int sum; // ����(��ģ) �簢�� ������ ��
    int lastTurn; // �ֱٿ� �簢���� ���� ��
    Board* board;

public:
    explicit Point(int x, int y, Board* board, State::Enum state = State::Normal);
    ~Point();

    // ��� ���� ���� ���� �Լ�
    inline int GetX() { return x; }
    inline int GetY() { return y; }
    inline bool IsDanger() { return state == State::Danger; }
    inline bool IsDead() { return state == State::Dead; }
    inline bool IsNormal() { return state == State::Normal; }
    inline int GetSum() { return sum; }
    inline void AddSum(const int number) { sum += number; }
    inline void SetBoard(Board* board) { this->board = board; }
    void ChangeState(State::Enum state, const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);

    // ���� ĭ ���� �Լ�
    inline shared_ptr<Point> GetLeft() { return board->GetPoint(x - 1, y); }
    inline shared_ptr<Point> GetRight() { return board->GetPoint(x + 1, y); }
    inline shared_ptr<Point> GetUp() { return board->GetPoint(x, y - 1); }
    inline shared_ptr<Point> GetDown() { return board->GetPoint(x, y + 1); }
    inline shared_ptr<Point> GetRightDown() { return board->GetPoint(x + 1, y + 1); }

    // �簢���� ����
    bool PutSquare(const int number);
    // �簢���� �������� ����(���� ��ȭ üũ)
    ExpectPutdown::Enum CheckPutSquare(const int number,
        int* clearCnt, int* dangerCnt, int* deadCnt);

private:
    // �簢���� ���� �� �ִ� �ڸ����� Ȯ��
    bool IsUnputdownable();
    bool IsUnputdownTurn();

    // ���� ĭ�� ���� ���¿� ���̴� ��Ȳ���� Ȯ�� �� ����
    void CheckAdjDead(shared_ptr<Point> checkPoint, const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
    // �ڽ��� ���� ���¿� ���̴� ��Ȳ���� Ȯ�� �� ����
    void CheckDanger(const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
    // �ڽ��� ���� ���¸� ������ �� �ִ� ��Ȳ���� Ȯ�� �� ����
    void CheckClearDanger(const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
};

