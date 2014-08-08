#pragma once
#include <set>
#include <vector>
#include <mutex>
#include "Board.h"

class AutoUser
{
private:
    unique_ptr<Board> board;
    static int maxSquareSum;
    static mutex _mutex;
    int threadNum;

    struct PutdownInfo
    {
        int idx;
        shared_ptr<Point> point;
        int clearCnt, dangerCnt, deadCnt;
        PutdownInfo(int idx, shared_ptr<Point> point)
            : idx(idx), point(point), clearCnt(0), dangerCnt(0), deadCnt(0) { }
    };

    // 아래의 set들은 2x2 사각형의 좌상단 점을 저장하며
    // 2x2의 사각형 크기로 인해 (0,0) ~ (7,7) 까지의 Point를 저장함

    // ExpectPutdown::Enum에 따라서 컨테이너에 저장
    // 놓을 수 있는 Point의 vector (매턴 초기화)
    vector<PutdownInfo> clearDangerableVect;    // 1순위
    vector<PutdownInfo> noproblemVect;          // 2순위
    vector<PutdownInfo> dangerableVect;         // 3순위
    vector<PutdownInfo> deadableVect;           // 4순위

    set<shared_ptr<Point>> putdownableSet;      // 값 유지
    set<shared_ptr<Point>> unputdownableSet;    // 값 유지
    set<shared_ptr<Point>> unputdownTurnSet;    // 매턴 초기화

public:
    explicit AutoUser(int threadNum);
    ~AutoUser();

    void NewGame();
    void AutoPlay();
    //void AutoPlay2();
};

