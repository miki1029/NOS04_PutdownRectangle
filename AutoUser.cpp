#include <iostream>
#include <string>
#include <algorithm>
#include "AutoUser.h"
#include "Point.h"

AutoUser::AutoUser(unique_ptr<Board> board)
: board(move(board)), maxSquareSum(0)
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
    cout << "새 게임 시작" << endl;
}


void AutoUser::AutoPlay()
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
}


void AutoUser::AutoPlay2()
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
        vector<PutdownInfo>::iterator targetItr;
        if (!clearDangerableVect.empty())
        {
            // 가장 위험상태를 많이 해제할 수 있는 것을 선택
            targetItr = max_element(
                clearDangerableVect.begin(), clearDangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) {
                return left.clearCnt < right.clearCnt;
            });
        }
        else if (!noproblemVect.empty())
        {
            // 가장 높은 숫자를 선택
            targetItr = max_element(
                noproblemVect.begin(), noproblemVect.end(),
                [this](const PutdownInfo& left, const PutdownInfo& right) -> bool {
                array<int, Board::BufferSize> hand = board->GetSquareHand();
                return hand[left.idx] < hand[right.idx];
            });
        }
        else if (!dangerableVect.empty())
        {
            // 가장 위험상태를 적게 만드는 것을 선택
            targetItr = min_element(
                dangerableVect.begin(), dangerableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) {
                return left.dangerCnt < right.dangerCnt;
            });
        }
        else if (!deadableVect.empty())
        {
            // 가장 죽음상태를 적게 만드는 것을 선택
            targetItr = min_element(
                deadableVect.begin(), deadableVect.end(),
                [](const PutdownInfo& left, const PutdownInfo& right) {
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

    // 결과 저장
    int squareSum = board->GetSquareSum();
    if (squareSum > 10000)
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
}