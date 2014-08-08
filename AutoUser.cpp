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

    // set 초기화
    for (int x = 0; x < Board::XSize - 1; x++)
    {
        for (int y = 0; y < Board::YSize - 1; y++)
        {
            // 모서리 부분은 처음부터 죽었기 때문에 놓을 수 없음
            if ((x == 0 && y == 0) ||
                (x == 0 && y == Board::YSize - 2) ||
                (x == Board::XSize - 2 && y == 0) ||
                (x == Board::XSize - 2 && y == Board::YSize - 2))
            {
                unputdownableSet.insert(board->GetPoint(x, y));
            }
            // 나머지는 놓아도 아무런 변화를 주지 않음
            else
            {
                putdownableSet.insert(board->GetPoint(x, y));
            }
        }
    }
}


// 우선순위에 따라 사각형을 놓음.
// 우선순위가 같은 부분에 대해서 차이가 발생하여 랜덤으로 다른 값이 나옴
// 최대 68181까지 나왔음(13635개)
void AutoUser::AutoPlay()
{
    while (true)
    {
        set<shared_ptr<Point>> eraseSet;

        // 놓을 수 있는 자리들에 대한 정보 초기화
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

        // 놓을 자리를 선정
        // 비교값이 같은 부분에 대해 랜덤으로 값이 정해짐
        vector<PutdownInfo>::iterator targetItr;
        if (!clearDangerableVect.empty())
        {
            // 가장 위험상태를 많이 해제할 수 있는 것을 선택
            targetItr = max_element(
                clearDangerableVect.begin(), clearDangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.clearCnt < right.clearCnt;
            });
        }
        else if (!noproblemVect.empty())
        {
            // 가장 높은 숫자를 선택
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
            // 가장 위험상태를 적게 만드는 것을 선택
            targetItr = min_element(
                dangerableVect.begin(), dangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.dangerCnt < right.dangerCnt;
            });
        }
        else if (!deadableVect.empty())
        {
            // 가장 죽음상태를 적게 만드는 것을 선택
            targetItr = min_element(
                deadableVect.begin(), deadableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) -> bool
            {
                return left.deadCnt < right.deadCnt;
            });
        }
        // 놓을 수 있는 자리가 없음
        else
        {
            break;
        }

        // targetItr의 정보에 따라 사각형을 놓음
        int idx = targetItr->idx;
        int x = targetItr->point->GetX();
        int y = targetItr->point->GetY();

        if (!board->UseSquare(idx, x, y))
            throw targetItr;

        // 턴 초기화
        clearDangerableVect.clear();
        noproblemVect.clear();
        dangerableVect.clear();
        deadableVect.clear();
        unputdownTurnSet.clear();
    }

    // acquire lock
    _mutex.lock();
    // 결과 저장(critical section)
    int squareSum = board->GetSquareSum();
    //cout << threadNum << ":" << squareSum << endl;
    if (squareSum > maxSquareSum)
    {
        board->SaveToFile();
        maxSquareSum = squareSum;
        cout << threadNum << ":" << squareSum << endl;
        // 만점 조건->종료
        if (board->GetOutput().size() >= 10000)
        {
            //cout << threadNum << ":" << squareSum << endl;
            exit(0); // 1만개 이상의 결과값이 나오면 종료
        }
    }
    // release lock
    _mutex.unlock();
}


// 완전 랜덤으로 짠 구조 최대 1738까지 나왔었음
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