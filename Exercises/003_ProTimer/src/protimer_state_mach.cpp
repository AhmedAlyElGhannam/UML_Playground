#include "main.h"

static void disp_time(uint32_t time)
{

}

static void disp_msg(String str)
{

}

static void disp_clr()
{

}

static void do_beep()
{

}

void protimer_init(protimer_t *const mobj)
{
    mobj->active_state = IDLE;
    mobj->productive_time = 0;
}

static EVENT_STATUS_t protimer_IDLE_state_handler(protimer_t *const mobj, const event_t *const e)
{
    switch (e->sig)
    {
        case ENTRY: 
            mobj->counter_time = 0; // action
            mobj->elapsed_time = 0; // action
            disp_time(0); // action
            disp_msg("Set Time"); // action
        return EVENT_HANDLED; // no state transition

        case EXIT: 
            disp_clr(); // action
        return EVENT_HANDLED; // no state transition

        case INC_TIME:
            mobj->counter_time += 60; // action
            mobj->active_state = TIME_SET; // state transition
        return EVENT_TRANSITION; // went to TIME_SET

        case START_PAUSE:
            // no action
            mobj->active_state = STAT; // state transition
        return EVENT_TRANSITION; // went to STAT

        case TIME_TICK:
            if (((tick_event_t *)(e))->ss == 5) // guard
            {
                do_beep(); // action
                return EVENT_HANDLED; // no state transition
            }
        return EVENT_IGNORED; // guard condition not fulfilled

        default:
            // no action
            // no transition
        return EVENT_IGNORED;
    }

    return EVENT_IGNORED;
}

static EVENT_STATUS_t protimer_TIME_SET_state_handler(protimer_t *const mobj, const event_t *const e)
{
    switch (e->sig)
    {
        case ENTRY: 
            disp_time(mobj->counter_time); // action
        return EVENT_HANDLED; // no state transition

        case INC_TIME:
            mobj->counter_time += 60; // action
            disp_time(mobj->counter_time); // action
        return EVENT_HANDLED; // no state transition

        case DEC_TIME:
            if (mobj->counter_time >= 60)
            {
                mobj->counter_time -= 60; // action
                disp_time(mobj->counter_time); // action
                return EVENT_HANDLED; // no state transition
            }
        return EVENT_IGNORED; // guard condition not fulfilled

        case START_PAUSE:
            // no action
            if (mobj->counter_time >= 60) // guard
            {
                mobj->active_state = COUNTDOWN; // state transition
                return EVENT_TRANSITION; // went to COUNTDOWN
            }
        return EVENT_IGNORED; // guard condition not fulfilled

        case ABRT:
            disp_clr(); // action
            mobj->active_state = IDLE; // state transition
        return EVENT_TRANSITION; // went to IDLE

        default:
            // no action
            // no transition
        return EVENT_IGNORED;
    }

    return EVENT_IGNORED;
}

static EVENT_STATUS_t protimer_COUNTDOWN_state_handler(protimer_t *const mobj, const event_t *const e)
{
    switch (e->sig)
    {
        case EXIT: 
            mobj->productive_time += mobj->elapsed_time; // action
            mobj->elapsed_time = 0; // action
        return EVENT_HANDLED; // no state transition

        case TIME_TICK:
            if (((tick_event_t *)(e))->ss == 10) // guard
            {
                --(mobj->counter_time); // action
                ++(mobj->elapsed_time); // action
                disp_time(mobj->counter_time); // action
                
                if ((mobj->counter_time) == 0)
                {
                    mobj->active_state = TIME_SET; // state transition
                    return EVENT_TRANSITION; // went to TIME_SET
                }
                
                return EVENT_HANDLED; // no state transition
            }
        return EVENT_IGNORED; 

        case START_PAUSE:
            // no action
            mobj->active_state = PAUSE; // state transition
        return EVENT_TRANSITION; // went to PAUSE

        case ABRT:
            // no action
            mobj->active_state = IDLE; // state transition
        return EVENT_TRANSITION; // went to IDLE

        default:
            // no action
            // no transition
        return EVENT_IGNORED;
    }

    return EVENT_IGNORED;
}

static EVENT_STATUS_t protimer_PAUSE_state_handler(protimer_t *const mobj, const event_t *const e)
{
    switch (e->sig)
    {
        case ENTRY: 
            disp_msg("Paused"); // action
        return EVENT_HANDLED; // no state transition
        
        case EXIT: 
            disp_clr(); // action
        return EVENT_HANDLED; // no state transition

        case INC_TIME:
            mobj->counter_time += 60; // action
            mobj->active_state = TIME_SET; // state transition
        return EVENT_TRANSITION; // went to TIME_SET

        case DEC_TIME:
            if (mobj->counter_time >= 60)
            {
                mobj->counter_time -= 60; // action
                mobj->active_state = TIME_SET; // state transition
                return EVENT_TRANSITION; // went to TIME_SET
            }
        return EVENT_IGNORED; // guard condition not fulfilled

        case START_PAUSE:
            // no action
            mobj->active_state = COUNTDOWN; // state transition
        return EVENT_TRANSITION; // went to COUNTDOWN

        case ABRT:
            // no action
            mobj->active_state = IDLE; // state transition
        return EVENT_TRANSITION; // went to IDLE

        default:
            // no action
            // no transition
        return EVENT_IGNORED;
    }

    return EVENT_IGNORED;
}

static EVENT_STATUS_t protimer_STAT_state_handler(protimer_t *const mobj, const event_t *const e)
{
    switch (e->sig)
    {
        case ENTRY: 
            disp_time(mobj->productive_time); // action
            disp_msg("Productive Time"); // action
        return EVENT_HANDLED; // no state transition
        
        case EXIT: 
            disp_clr(); // action
        return EVENT_HANDLED; // no state transition

        case TIME_TICK:
            if (((tick_event_t *)(e))->ss == 10) // guard
            {
                // no action
                mobj->active_state = IDLE; // state transition
                return EVENT_TRANSITION; // went to TIME_SET
            }
        return EVENT_IGNORED; 

        default:
            // no action
            // no transition
        return EVENT_IGNORED;
    }

    return EVENT_IGNORED;
}

EVENT_STATUS_t protimer_state_machine(protimer_t *const mobj, const event_t *const e)
{
    switch (mobj->active_state)
    {
        case IDLE: return protimer_IDLE_state_handler(mobj, e);

        case TIME_SET: return protimer_TIME_SET_state_handler(mobj, e);
        
        case COUNTDOWN: return protimer_COUNTDOWN_state_handler(mobj, e);

        case PAUSE: return protimer_PAUSE_state_handler(mobj, e);

        case STAT: return protimer_STAT_state_handler(mobj, e);
    }

    return EVENT_IGNORED;
}

