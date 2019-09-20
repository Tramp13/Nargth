#include "dreamwarp.h"

Timer Timer_create() {
    Timer timer;
    timer.start_ticks = 0;
    timer.paused_ticks = 0;
    timer.paused = false;
    timer.started = false;

    return timer;
}

void Timer_start(Timer *timer) {
    timer->started = true;
    timer->paused = false;

    timer->start_ticks = SDL_GetTicks();
}

void Timer_stop(Timer *timer) {
    timer->started = false;
    timer->paused = false;
}

void Timer_pause(Timer *timer) {
    if ((timer->started == true) && (timer->paused == false)) {
        timer->paused = true;
        timer->paused_ticks = SDL_GetTicks() - timer->start_ticks;
    }
}

void Timer_unpause(Timer *timer) {
    if (timer->paused == true) {
        timer->paused = false;
        timer->start_ticks = SDL_GetTicks() - timer->paused_ticks;
        timer->paused_ticks = 0;
    }
}

int Timer_get_ticks(Timer *timer) {
    if (timer->started == true) {
        if (timer->paused == true)
            return timer->paused_ticks;
    } else {
        return SDL_GetTicks() - timer->start_ticks;
    }

    // if timer isn't running
    return 0;
}

bool Timer_is_started(Timer *timer) {
    return timer->started;
}

bool Timer_is_paused(Timer *timer) {
    return timer->paused;
}
