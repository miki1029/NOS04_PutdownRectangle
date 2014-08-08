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

    // �Ʒ��� set���� 2x2 �簢���� �»�� ���� �����ϸ�
    // 2x2�� �簢�� ũ��� ���� (0,0) ~ (7,7) ������ Point�� ������

    // ExpectPutdown::Enum�� ���� �����̳ʿ� ����
    // ���� �� �ִ� Point�� vector (���� �ʱ�ȭ)
    vector<PutdownInfo> clearDangerableVect;    // 1����
    vector<PutdownInfo> noproblemVect;          // 2����
    vector<PutdownInfo> dangerableVect;         // 3����
    vector<PutdownInfo> deadableVect;           // 4����

    set<shared_ptr<Point>> putdownableSet;      // �� ����
    set<shared_ptr<Point>> unputdownableSet;    // �� ����
    set<shared_ptr<Point>> unputdownTurnSet;    // ���� �ʱ�ȭ

public:
    explicit AutoUser(int threadNum);
    ~AutoUser();

    void NewGame();
    void AutoPlay();
    //void AutoPlay2();
};

