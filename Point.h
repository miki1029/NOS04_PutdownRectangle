#pragma once
#include "defEnum.h"
#include "Board.h"

using namespace std;

class Point
{
private:
    int x, y;
    State::Enum state; // Normal, Danger, Dead
    int sum; // 놓인(겹친) 사각형 숫자의 합
    int lastTurn; // 최근에 사각형이 놓인 턴
    Board* board;

public:
    explicit Point(int x, int y, Board* board, State::Enum state = State::Normal);
    ~Point();

    // 멤버 변수 접근 관련 함수
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

    // 인접 칸 접근 함수
    inline shared_ptr<Point> GetLeft() { return board->GetPoint(x - 1, y); }
    inline shared_ptr<Point> GetRight() { return board->GetPoint(x + 1, y); }
    inline shared_ptr<Point> GetUp() { return board->GetPoint(x, y - 1); }
    inline shared_ptr<Point> GetDown() { return board->GetPoint(x, y + 1); }
    inline shared_ptr<Point> GetRightDown() { return board->GetPoint(x + 1, y + 1); }

    // 사각형을 놓음
    bool PutSquare(const int number);
    // 사각형을 가상으로 놓음(상태 변화 체크)
    ExpectPutdown::Enum CheckPutSquare(const int number,
        int* clearCnt, int* dangerCnt, int* deadCnt);

private:
    // 사각형을 놓을 수 있는 자리인지 확인
    bool IsUnputdownable();
    bool IsUnputdownTurn();

    // 인접 칸이 죽은 상태에 놓이는 상황인지 확인 후 설정
    void CheckAdjDead(shared_ptr<Point> checkPoint, const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
    // 자신이 위험 상태에 놓이는 상황인지 확인 후 설정
    void CheckDanger(const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
    // 자신이 위험 상태를 해제할 수 있는 상황인지 확인 후 설정
    void CheckClearDanger(const bool checkOnly = false,
        int* clearCnt = nullptr, int* dangerCnt = nullptr, int* deadCnt = nullptr);
};

