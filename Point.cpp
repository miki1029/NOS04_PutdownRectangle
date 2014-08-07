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
    // �̹� ���� ĭ�̸� ����
    if (this->state == State::Dead) return;
    if (this->state != state)
    {
        cout << "    (" << x << ", " << y << ") ";
        switch (this->state)
        {
        case State::Normal:
            cout << "����->";
            break;
        case State::Danger:
            cout << "����->";
            break;
        case State::Dead:
            cout << "����->";
            break;
        default:
            break;
        }
        switch (state)
        {
        case State::Normal:
            cout << "����" << endl;
            break;
        case State::Danger:
            cout << "����" << endl;
            break;
        case State::Dead:
            cout << "����" << endl;
            break;
        default:
            break;
        }
    }
    this->state = state;
    // ���� �Ǵ� ���� ���·� �ٲ�� ���� ĭ�� �״��� Ȯ��
    if (state != State::Normal)
    {
        CheckAdjDead(GetLeft());
        CheckAdjDead(GetRight());
        CheckAdjDead(GetUp());
        CheckAdjDead(GetDown());
    }
    // ����ĭ�� ������ ���� ��� ������°� �ȴ�.
    if (state == State::Dead)
    {
        board->IncDeadCnt(); // ���� ĭ �� ����
        GetLeft()->ChangeState(State::Danger);
        GetRight()->ChangeState(State::Danger);
        GetUp()->ChangeState(State::Danger);
        GetDown()->ChangeState(State::Danger);
    }
}

bool Point::PutSquare(const int number)
{
    // �簢���� ���� �� ���� �ڸ��� ���
    if (!CheckPutSquare())
    {
        return false;
    }

    cout << number << " (" << x << ", " << y << ")" << endl;
    // ���� ���� ������
    lastTurn = board->GetTurn();

    // �簢�� ������ �� ����
    AddSum(number);
    GetRight()->AddSum(number);
    GetDown()->AddSum(number);
    GetRightDown()->AddSum(number);

    // �̿��� ���ڿ��� ���� 10 �̻��̸鼭 2�� �̻��� �Ǹ�
    // ���ڰ� ū ĭ�� ���� ���¿� ���̰� �ȴ�.
    // -> �簢���� ���� ��ġ�� ���� ���°� �� ���ɼ��� ����.
    CheckDanger();
    GetRight()->CheckDanger();
    GetDown()->CheckDanger();
    GetRightDown()->CheckDanger();

    // ���� ���¿��� ���� �ʿ� �簢���� ���ļ�
    // ���̰� �ٰų� 2�谡 �ȵǸ� ���� ���´� �����ȴ�.
    // -> �簢���� ���� ��ġ�� ������ ��ġ�� ���� ���°� ������ ���ɼ��� ����.
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

    // ����ĭ���� �簢���� ���ĳ��� �� ������,
    // ����ĭ�� ���ĳ��� ��� �ش� �簢���� ���� ĭ�� ��� ����ĭ�� �ȴ�.
    // -> �ƿ� ���� �ʴ� ������ ó��
    if (IsDead() ||
        GetRight()->IsDead() ||
        GetDown()->IsDead() ||
        GetRightDown()->IsDead())
    {
        /*cout << "���� ĭ�� ���� ���� �� ����";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return false;
        /*ChangeState(State::Dead);
        GetRight()->ChangeState(State::Dead);
        GetDown()->ChangeState(State::Dead);
        GetRightDown()->ChangeState(State::Dead);
        return true;*/
    }
    // ���� 16�� ���� ���Ҵ� ��ǥ�� �簢���� ���� ���
    // �ش� �簢���� ���� ĭ�� ��� ����ĭ�� �ȴ�.
    // -> �ƿ� ���� �ʴ� ������ ó��
    else if (lastTurn != 0 && lastTurn + Board::InputSize > board->GetTurn() - 1)
    {
        /*cout << (lastTurn + Board::InputSize) - (board->GetTurn() - 1) << "�� �� ���� �� �ִ� �ڸ� ";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return false;
    }
    // ������ ĭ�� ��� ����ĭ�� ������ �簢���� ���� �� ����.
    else if (GetLeft()->IsDead() &&
        GetUp()->IsDead() &&
        GetRight()->GetUp()->IsDead() &&
        GetRight()->GetRight()->IsDead() &&
        GetDown()->GetLeft()->IsDead() &&
        GetDown()->GetDown()->IsDead() &&
        GetRightDown()->GetRight()->IsDead() &&
        GetRightDown()->GetDown()->IsDead())
    {
        /*cout << "���� ĭ�� ��� ����";
        cout << " (" << x << ", " << y << ")" << endl;*/
        return false;
    }
    return true;
}

void Point::CheckAdjDead(shared_ptr<Point> checkPoint)
{
    // �̹� ���� ���̸� Ȯ�� ���ʿ�
    if (checkPoint->IsDead()) return;

    // ������ 4�� ��� ���� �Ǵ� ����ĭ ���°� �Ǹ� �ش� ĭ�� ����ĭ�� �ȴ�.
    if (!checkPoint->GetLeft()->IsNormal() &&
        !checkPoint->GetRight()->IsNormal() &&
        !checkPoint->GetUp()->IsNormal() &&
        !checkPoint->GetDown()->IsNormal())
    {
        checkPoint->ChangeState(State::Dead);
    }

    // ���Ʒ� Ȥ�� �¿��� ĭ�� ��� ����ĭ�̸� �� ĭ�� ����ĭ�� �ȴ�.
    if ((checkPoint->GetUp()->IsDead() && checkPoint->GetDown()->IsDead()) ||
        (checkPoint->GetLeft()->IsDead() && checkPoint->GetRight()->IsDead()))
    {
        checkPoint->ChangeState(State::Dead);
    }
}
void Point::CheckDanger()
{
    // �̿��� ���ڿ��� ���� 10 �̻��̸鼭 2�� �̻��� �Ǹ�
    // ���ڰ� ū ĭ�� ���� ���¿� ���̰� �ȴ�.
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
    // �̹� ���� ���°� �ƴϸ� ����
    if (!IsDanger() ||
        // ���� �����̰� ���� ĭ�� �׾������� ����
        GetLeft()->IsDead() ||
        GetRight()->IsDead() ||
        GetUp()->IsDead() ||
        GetDown()->IsDead())
    {
        return;
    }

    // ���� ���¿��� ���� �ʿ� �簢���� ���ļ�
    // ���̰� �ٰų� 2�谡 �ȵǰ� �Ǹ� ���� ���´� �����ȴ�.
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