#include <iostream>
#include <cassert>
#include "Point.h"

Point::Point(int x, int y, Board* board, State::Enum state)
: x(x), y(y), board(board), state(state), sum(0), lastTurn(0)
{
}


Point::~Point()
{
}


void Point::ChangeState(State::Enum state)
{
    // 이미 죽은 칸이면 무시
    if (this->state == State::Dead) return;
    if (this->state != state)
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
    }
    this->state = state;
    // 위험 또는 죽은 상태로 바뀌면 인접 칸이 죽는지 확인
    if (state != State::Normal)
    {
        CheckAdjDead(GetLeft());
        CheckAdjDead(GetRight());
        CheckAdjDead(GetUp());
        CheckAdjDead(GetDown());
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
    if (!CheckPutSquare())
    {
        return false;
    }

    cout << number << " (" << x << ", " << y << ")" << endl;
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

bool Point::CheckPutSquare()
{
    assert(x >= 0 && x < board->XSize - 1 && y >= 0 && y < board->YSize - 1);

    // 죽은칸에는 사각형을 겹쳐놓을 수 없으며,
    // 죽은칸에 겹쳐놓을 경우 해당 사각형이 놓인 칸이 모두 죽은칸이 된다.
    // -> 아에 놓지 않는 것으로 처리
    if (IsDead() ||
        GetRight()->IsDead() ||
        GetDown()->IsDead() ||
        GetRightDown()->IsDead())
    {
        /*cout << "죽은 칸에 겹쳐 놓을 수 없음";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return false;
        /*ChangeState(State::Dead);
        GetRight()->ChangeState(State::Dead);
        GetDown()->ChangeState(State::Dead);
        GetRightDown()->ChangeState(State::Dead);
        return true;*/
    }
    // 직전 16번 동안 놓았던 좌표에 사각형을 놓을 경우
    // 해당 사각형이 놓인 칸은 모두 죽은칸이 된다.
    // -> 아에 놓지 않는 것으로 처리
    else if (lastTurn != 0 && lastTurn + Board::InputSize > board->GetTurn() - 1)
    {
        /*cout << (lastTurn + Board::InputSize) - (board->GetTurn() - 1) << "턴 후 놓을 수 있는 자리 ";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return false;
    }
    // 인접한 칸이 모두 죽은칸인 곳에는 사각형을 놓을 수 없다.
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
        return false;
    }
    return true;
}

void Point::CheckAdjDead(shared_ptr<Point> checkPoint)
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
void Point::CheckDanger()
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
        ChangeState(State::Danger);
    }
}
void Point::CheckClearDanger()
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
        ChangeState(State::Normal);
    }
}