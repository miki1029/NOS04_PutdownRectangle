//#include <iostream>
#include <cassert>
#include "Point.h"

Point::Point(int x, int y, Board* board, State::Enum state)
: x(x), y(y), board(board), state(state), sum(0), lastTurn(0)
{
}


Point::~Point()
{
}


void Point::ChangeState(State::Enum state, const bool checkOnly,
    int* clearCnt, int* dangerCnt, int* deadCnt)
{
    // 이미 죽은 칸이면 무시
    if (this->state == State::Dead) return;
    /*if (this->state != state)
    {
        cout << "    (" << x << ", " << y << ") ";
        switch (this->state)
        {
        case State::Normal:
            cout << "보통->";
            break;
        case State::Danger:
            cout << "위험->";
            break;
        case State::Dead:
            cout << "죽음->";
            break;
        default:
            break;
        }
        switch (state)
        {
        case State::Normal:
            cout << "보통" << endl;
            break;
        case State::Danger:
            cout << "위험" << endl;
            break;
        case State::Dead:
            cout << "죽음" << endl;
            break;
        default:
            break;
        }
    }*/
    // 체크용인 경우 상태변화 수 증가
    if (checkOnly && this->state != state)
    {
        switch (state)
        {
        case State::Normal:
            (*clearCnt)++;
            break;
        case State::Danger:
            (*dangerCnt)++;
            break;
        case State::Dead:
            (*deadCnt)++;
            break;
        default:
            break;
        }
    }
    this->state = state;
    // 위험 또는 죽은 상태로 바뀌면 인접 칸이 죽는지 확인
    if (state != State::Normal)
    {
        CheckAdjDead(GetLeft(), checkOnly, clearCnt, dangerCnt, deadCnt);
        CheckAdjDead(GetRight(), checkOnly, clearCnt, dangerCnt, deadCnt);
        CheckAdjDead(GetUp(), checkOnly, clearCnt, dangerCnt, deadCnt);
        CheckAdjDead(GetDown(), checkOnly, clearCnt, dangerCnt, deadCnt);
    }
    // 죽은칸과 인접한 면은 모두 위험상태가 된다.
    if (state == State::Dead)
    {
        board->IncDeadCnt(); // 죽은 칸 수 증가
        GetLeft()->ChangeState(State::Danger);
        GetRight()->ChangeState(State::Danger);
        GetUp()->ChangeState(State::Danger);
        GetDown()->ChangeState(State::Danger);
    }
}


bool Point::PutSquare(const int number)
{
    // 사각형을 놓을 수 없는 자리인 경우
    if (IsUnputdownable() || IsUnputdownTurn())
        return false;

    //cout << number << " (" << x << ", " << y << ")" << endl;
    // 현재 턴을 저장함
    lastTurn = board->GetTurn();

    // 사각형 숫자의 합 증가
    AddSum(number);
    GetRight()->AddSum(number);
    GetDown()->AddSum(number);
    GetRightDown()->AddSum(number);

    // 이웃한 숫자와의 차가 10 이상이면서 2배 이상이 되면
    // 숫자가 큰 칸은 위험 상태에 놓이게 된다.
    // -> 사각형이 놓인 위치가 위험 상태가 될 가능성이 있음.
    CheckDanger();
    GetRight()->CheckDanger();
    GetDown()->CheckDanger();
    GetRightDown()->CheckDanger();

    // 위험 상태에서 작은 쪽에 사각형을 겹쳐서
    // 차이가 줄거나 2배가 안되면 위험 상태는 해제된다.
    // -> 사각형이 놓인 위치와 인접한 위치가 위험 상태가 해제될 가능성이 있음.
    GetLeft()->CheckClearDanger();
    GetUp()->CheckClearDanger();
    GetRight()->GetUp()->CheckClearDanger();
    GetRight()->GetRight()->CheckClearDanger();
    GetDown()->GetLeft()->CheckClearDanger();
    GetDown()->GetDown()->CheckClearDanger();
    GetRightDown()->GetRight()->CheckClearDanger();
    GetRightDown()->GetDown()->CheckClearDanger();

    return true;
}


ExpectPutdown::Enum Point::CheckPutSquare(const int number, int* clearCnt, int* dangerCnt, int* deadCnt)
{
    assert(x >= 0 && x < board->XSize - 1 && y >= 0 && y < board->YSize - 1);

    // 놓지 못하는 칸인 경우
    if (IsUnputdownable())
        return ExpectPutdown::Unputdownable;
    else if (IsUnputdownTurn())
        return ExpectPutdown::UnputdownTurn;

    // 판의 사본을 만듦
    Board testBoard = *board;
    shared_ptr<Point> checkPoint = testBoard.GetPoint(x, y);

    // 판의 사본에 사각형을 놓음
    // 사각형 숫자의 합 증가
    checkPoint->AddSum(number);
    checkPoint->GetRight()->AddSum(number);
    checkPoint->GetDown()->AddSum(number);
    checkPoint->GetRightDown()->AddSum(number);

    // 이웃한 숫자와의 차가 10 이상이면서 2배 이상이 되면
    // 숫자가 큰 칸은 위험 상태에 놓이게 된다.
    // -> 사각형이 놓인 위치가 위험 상태가 될 가능성이 있음.
    checkPoint->CheckDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRight()->CheckDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetDown()->CheckDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRightDown()->CheckDanger(true, clearCnt, dangerCnt, deadCnt);

    // 위험 상태에서 작은 쪽에 사각형을 겹쳐서
    // 차이가 줄거나 2배가 안되면 위험 상태는 해제된다.
    // -> 사각형이 놓인 위치와 인접한 위치가 위험 상태가 해제될 가능성이 있음.
    checkPoint->GetLeft()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetUp()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRight()->GetUp()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRight()->GetRight()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetDown()->GetLeft()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetDown()->GetDown()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRightDown()->GetRight()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);
    checkPoint->GetRightDown()->GetDown()->CheckClearDanger(true, clearCnt, dangerCnt, deadCnt);

    if (*deadCnt > 0)
        return ExpectPutdown::Deadable;
    else if (*dangerCnt > 0)
        return ExpectPutdown::Dangerable;
    else if (*clearCnt > 0)
        return ExpectPutdown::ClearDangerable;
    else
        return ExpectPutdown::Noproblem;
}


bool Point::IsUnputdownable()
{
    // 죽은칸에는 사각형을 겹쳐놓을 수 없으며,
    // 죽은칸에 겹쳐놓을 경우 해당 사각형이 놓인 칸이 모두 죽은칸이 된다.
    // -> 아에 놓지 못하는 것으로 처리
    if (IsDead() ||
        GetRight()->IsDead() ||
        GetDown()->IsDead() ||
        GetRightDown()->IsDead())
    {
        /*cout << "죽은 칸에 겹쳐 놓을 수 없음";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return true;
        /*ChangeState(State::Dead);
        GetRight()->ChangeState(State::Dead);
        GetDown()->ChangeState(State::Dead);
        GetRightDown()->ChangeState(State::Dead);
        return false;*/
    }
    // 인접한 칸이 모두 죽은칸인 곳에는 사각형을 놓을 수 없다.
    // -> 아에 놓지 못하는 것으로 처리
    else if (GetLeft()->IsDead() &&
        GetUp()->IsDead() &&
        GetRight()->GetUp()->IsDead() &&
        GetRight()->GetRight()->IsDead() &&
        GetDown()->GetLeft()->IsDead() &&
        GetDown()->GetDown()->IsDead() &&
        GetRightDown()->GetRight()->IsDead() &&
        GetRightDown()->GetDown()->IsDead())
    {
        /*cout << "인접 칸이 모두 죽음";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return true;
    }
    return false;
}


bool Point::IsUnputdownTurn()
{
    // 직전 16번 동안 놓았던 좌표에 사각형을 놓을 경우
    // 해당 사각형이 놓인 칸은 모두 죽은칸이 된다.
    // -> 아직 놓지 못하는 것으로 처리(나중에 놓을 수 있음)
    if (lastTurn != 0 && lastTurn + Board::InputSize > board->GetTurn() - 1)
    {
        /*cout << (lastTurn + Board::InputSize) - (board->GetTurn() - 1) << "턴 후 놓을 수 있는 자리 ";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return true;
    }
    return false;
}


void Point::CheckAdjDead(shared_ptr<Point> checkPoint, const bool checkOnly,
    int* clearCnt, int* dangerCnt, int* deadCnt)
{
    // 이미 죽은 점이면 확인 불필요
    if (checkPoint->IsDead()) return;

    // 인접한 4면 모두 위험 또는 죽은칸 상태가 되면 해당 칸은 죽은칸이 된다.
    if (!checkPoint->GetLeft()->IsNormal() &&
        !checkPoint->GetRight()->IsNormal() &&
        !checkPoint->GetUp()->IsNormal() &&
        !checkPoint->GetDown()->IsNormal())
    {
        checkPoint->ChangeState(State::Dead);
    }

    // 위아래 혹은 좌우의 칸이 모두 죽은칸이면 이 칸도 죽은칸이 된다.
    if ((checkPoint->GetUp()->IsDead() && checkPoint->GetDown()->IsDead()) ||
        (checkPoint->GetLeft()->IsDead() && checkPoint->GetRight()->IsDead()))
    {
        checkPoint->ChangeState(State::Dead);
    }
}


void Point::CheckDanger(const bool checkOnly, int* clearCnt, int* dangerCnt, int* deadCnt)
{
    // 이웃한 숫자와의 차가 10 이상이면서 2배 이상이 되면
    // 숫자가 큰 칸은 위험 상태에 놓이게 된다.
    // [if m-n>=10 && m>=2*n then m.danger = true; end if]
    int m = GetSum();
    int n1 = GetLeft()->GetSum();
    int n2 = GetRight()->GetSum();
    int n3 = GetUp()->GetSum();
    int n4 = GetDown()->GetSum();
    if ((m - n1 >= 10 && m >= 2 * n1) ||
        (m - n2 >= 10 && m >= 2 * n2) ||
        (m - n3 >= 10 && m >= 2 * n3) ||
        (m - n4 >= 10 && m >= 2 * n4))
    {
        ChangeState(State::Danger, checkOnly, clearCnt, dangerCnt, deadCnt);
    }
}


void Point::CheckClearDanger(const bool checkOnly, int* clearCnt, int* dangerCnt, int* deadCnt)
{
    // 이미 위험 상태가 아니면 무시
    if (!IsDanger() ||
        // 위험 상태이고 인접 칸이 죽어있으면 무시
        GetLeft()->IsDead() ||
        GetRight()->IsDead() ||
        GetUp()->IsDead() ||
        GetDown()->IsDead())
    {
        return;
    }

    // 위험 상태에서 작은 쪽에 사각형을 겹쳐서
    // 차이가 줄거나 2배가 안되게 되면 위험 상태는 해제된다.
    int m = GetSum();
    int n1 = GetLeft()->GetSum();
    int n2 = GetRight()->GetSum();
    int n3 = GetUp()->GetSum();
    int n4 = GetDown()->GetSum();
    if (!(m - n1 >= 10 && m >= 2 * n1) &&
        !(m - n2 >= 10 && m >= 2 * n2) &&
        !(m - n3 >= 10 && m >= 2 * n3) &&
        !(m - n4 >= 10 && m >= 2 * n4))
    {
        ChangeState(State::Normal, checkOnly, clearCnt, dangerCnt, deadCnt);
    }
}