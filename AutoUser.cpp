#include <iostream>
//#include <string>
#include <algorithm>
#include "AutoUser.h"
#include "Point.h"

int AutoUser::maxSquareSum = 0;
mutex AutoUser::_mutex;

AutoUser::AutoUser(int threadNum)
: threadNum(threadNum)
{
}


AutoUser::~AutoUser()
{
}


void AutoUser::NewGame()
{
    board = make_unique<Board>();

    clearDangerableVect.clear();
    noproblemVect.clear();
    noproblemVect.clear();
    deadableVect.clear();

    putdownableSet.clear();
    unputdownableSet.clear();
    unputdownTurnSet.clear();

    // set �ʱ�ȭ
    for (int x = 0; x < Board::XSize - 1; x++)
    {
        for (int y = 0; y < Board::YSize - 1; y++)
        {
            // �𼭸� �κ��� ó������ �׾��� ������ ���� �� ����
            if ((x == 0 && y == 0) ||
                (x == 0 && y == Board::YSize - 2) ||
                (x == Board::XSize - 2 && y == 0) ||
                (x == Board::XSize - 2 && y == Board::YSize - 2))
            {
                unputdownableSet.insert(board->GetPoint(x, y));
            }
            // �������� ���Ƶ� �ƹ��� ��ȭ�� ���� ����
            else
            {
                putdownableSet.insert(board->GetPoint(x, y));
            }
        }
    }
}


// �켱������ ���� �簢���� ����.
// �켱������ ���� �κп� ���ؼ� ���̰� �߻��Ͽ� �������� �ٸ� ���� ����
// �ִ� 68181���� ������(13635��)
void AutoUser::AutoPlay()
{
    while (true)
    {
        set<shared_ptr<Point>> eraseSet;

        // ���� �� �ִ� �ڸ��鿡 ���� ���� �ʱ�ȭ
        for (int idx = 0; idx < Board::BufferSize; idx++)
        {
            for each (shared_ptr<Point> p in putdownableSet)
            {
                PutdownInfo info(idx, p);
                ExpectPutdown::Enum expectResult = board->CheckSquare(idx, p->GetX(), p->GetY(),
                    &info.clearCnt, &info.dangerCnt, &info.deadCnt);

                switch (expectResult)
                {
                case ExpectPutdown::ClearDangerable:
                    clearDangerableVect.push_back(info);
                    break;
                case ExpectPutdown::Noproblem:
                    noproblemVect.push_back(info);
                    break;
                case ExpectPutdown::Dangerable:
                    dangerableVect.push_back(info);
                    break;
                case ExpectPutdown::Deadable:
                    deadableVect.push_back(info);
                    break;
                case ExpectPutdown::UnputdownTurn:
                    unputdownTurnSet.insert(p);
                    break;
                case ExpectPutdown::Unputdownable:
                    unputdownableSet.insert(p);
                    eraseSet.insert(p);
                    break;
                default:
                    break;
                }
            }
        }
        for each (shared_ptr<Point> p in eraseSet)
        {
            putdownableSet.erase(p);
        }

        // ���� �ڸ��� ����
        // �񱳰��� ���� �κп� ���� �������� ���� ������
        vector<PutdownInfo>::iterator targetItr;
        if (!clearDangerableVect.empty())
        {
            // ���� ������¸� ���� ������ �� �ִ� ���� ����
            targetItr = max_element(
                clearDangerableVect.begin(), clearDangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.clearCnt < right.clearCnt;
            });
        }
        else if (!noproblemVect.empty())
        {
            // ���� ���� ���ڸ� ����
            targetItr = max_element(
                noproblemVect.begin(), noproblemVect.end(),
                [this](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                array<int, Board::BufferSize> hand = board->GetSquareHand();
                return hand[left.idx] < hand[right.idx];
            });
        }
        else if (!dangerableVect.empty())
        {
            // ���� ������¸� ���� ����� ���� ����
            targetItr = min_element(
                dangerableVect.begin(), dangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.dangerCnt < right.dangerCnt;
            });
        }
        else if (!deadableVect.empty())
        {
            // ���� �������¸� ���� ����� ���� ����
            targetItr = min_element(
                deadableVect.begin(), deadableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.deadCnt < right.deadCnt;
            });
        }
        // ���� �� �ִ� �ڸ��� ����
        else
        {
            break;
        }

        // targetItr�� ������ ���� �簢���� ����
        int idx = targetItr->idx;
        int x = targetItr->point->GetX();
        int y = targetItr->point->GetY();

        if (!board->UseSquare(idx, x, y))
            throw targetItr;

        // �� �ʱ�ȭ
        clearDangerableVect.clear();
        noproblemVect.clear();
        dangerableVect.clear();
        deadableVect.clear();
        unputdownTurnSet.clear();
    }

    // acquire lock
    _mutex.lock();
    // ��� ����(critical section)
    int squareSum = board->GetSquareSum();
    //cout << threadNum << ":" << squareSum << endl;
    if (squareSum > maxSquareSum)
    {
        board->SaveToFile();
        maxSquareSum = squareSum;
        cout << threadNum << ":" << squareSum << endl;
        // ���� ����->����
        if (board->GetOutput().size() >= 10000)
        {
            //cout << threadNum << ":" << squareSum << endl;
            exit(0); // 1���� �̻��� ������� ������ ����
        }
    }
    // release lock
    _mutex.unlock();
}


// ���� �������� § ���� �ִ� 1738���� ���Ծ���
/*void AutoUser::AutoPlay2()
{
while (unputdownableSet.size() < Board::PutdownableSize)
{
int idx = rand() % Board::BufferSize;
int x = rand() % (Board::XSize - 1);
int y = rand() % (Board::YSize - 1);

if (board->UseSquare(idx, x, y))
{
unputdownableSet.clear();
}
else
{
unputdownableSet.insert(board->GetPoint(x, y));
}
}

int squareSum = board->GetSquareSum();
if (squareSum > 1400)
{
string filename = to_string(squareSum) + ".txt";
cout << filename << endl;
board->SaveToFile(filename.c_str());
}
if (squareSum > maxSquareSum)
{
board->SaveToFile();
maxSquareSum = squareSum;
cout << maxSquareSum << endl;
}
}*/