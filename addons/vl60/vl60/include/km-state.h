#ifndef     KM_STATE_H
#define     KM_STATE_H

#include    <array>

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    POS_BV = 0,     // Быстрое выключение
    POS_ZERO = 1,   // Ноль
    POS_AV = 2,     // Автоматическое выключение
    POS_RV = 3,     // Ручное выключение
    POS_FV = 4,     // Фиксация выключения
    POS_FP = 5,     // Фиксация пуска
    POS_RP = 6,     // Ручной пуск
    POS_AP = 7     // Автоматический пуск
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
enum
{
    REVERS_BACKWARD = 0,
    REVERS_ZERO = 1,
    REVERS_FORWARD = 2,
    REVERS_OP1 = 3,
    REVERS_OP2 = 4,
    REVERS_OP3 = 5,
};

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
struct km_state_t
{
    /// Заданное контроллером направление движения
    int     revers_ref_state;

    enum
    {
        NUM_MAIN_POSITIONS = 8,
    };

    /// Ступень ослабления возбуждения
    int     field_loosen_pos;

    /// Состояние каждой из позиций главной рукоятки
    std::array<bool, NUM_MAIN_POSITIONS>    pos_state;


    km_state_t()
        : revers_ref_state(0)
        , field_loosen_pos(0)
    {
        std::fill(pos_state.begin(), pos_state.end(), false);
        pos_state[POS_ZERO] = true;
    }
};

#endif // KM_STATE_H
