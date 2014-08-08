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
        ClearDangerable,    // 위험 상태를 해제할 수 있음 (1순위)
        Noproblem,          // 아무 변화 없음 (2순위)
        Dangerable,         // 위험 상태 발생 (3순위)
        Deadable,           // 죽음 상태 발생 (4순위)
        UnputdownTurn,      // 아직 놓을 수 없는 턴(나중에 놓을 수 있음)
        Unputdownable       // 규칙에 의해 놓을 수 없음(아에 놓지 못함)
    };
}