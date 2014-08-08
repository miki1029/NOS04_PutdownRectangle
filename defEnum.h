#pragma once

namespace State
{
    enum Enum
    {
        Normal,
        Danger,
        Dead
    };
}

namespace ExpectPutdown
{
    enum Enum
    {
        ClearDangerable,    // ���� ���¸� ������ �� ���� (1����)
        Noproblem,          // �ƹ� ��ȭ ���� (2����)
        Dangerable,         // ���� ���� �߻� (3����)
        Deadable,           // ���� ���� �߻� (4����)
        UnputdownTurn,      // ���� ���� �� ���� ��(���߿� ���� �� ����)
        Unputdownable       // ��Ģ�� ���� ���� �� ����(�ƿ� ���� ����)
    };
}