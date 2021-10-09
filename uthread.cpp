#ifndef _UTHREAD_CPP_
#define _UTHREAD_CPP_

#include "uthread.h"

void uthread_body(schedule_t& st) {
    int id = st.running_thread;
    if(id != -1) {
        uthread_t *utt = &st.threads[id];

        utt->func(utt->arg);

        utt->state = FREE;

        st.running_thread = -1;
    }

}

int uthread_create(schedule_t &schedule, Fun func, void * arg) {
    int id;
    for(id = 0; id < schedule.max_index; id++) {
        if(schedule.threads[id].state == FREE)
            break;
    }

    if(id == schedule.max_index) {
        schedule.max_index++;
    }
    uthread_t *utt = &schedule.threads[id];
    
    utt->state = RUNNABLE;
    utt->func = func;
    utt->arg = arg;

    getcontext(&utt->ctx);

    utt->ctx.uc_stack.ss_sp = utt->stack;
    utt->ctx.uc_stack.ss_size = sizeof(utt->stack);
    utt->ctx.uc_flags = 0;
    utt->ctx.uc_link = &schedule.main;
    schedule.running_thread = id;

    makecontext(&(utt->ctx), (void(*)(void))uthread_body, 1, &schedule);
    swapcontext(&(schedule.main), &(utt->ctx));

    return id;
}

void uthread_yield(schedule_t &schedule) {
    if(schedule.running_thread != -1) {
        uthread_t * utt = &schedule.threads[schedule.running_thread];
        utt->state = SUSPEND;
        schedule.running_thread = -1;
        swapcontext(&(utt->ctx), &(schedule.main));
    }
}


void uthread_resume(schedule_t &schedule, int id) {
    if(id < 0 || id >= schedule.max_index)
        return;
    uthread_t * utt = &schedule.threads[id];
    if(utt->state = SUSPEND) {
        swapcontext(&(schedule.main), &(utt->ctx));
    }
}

int schedule_finished(const schedule_t & schedule) {
    if(schedule.running_thread != -1) {
        return 0;
    } else {
        for(int i = 0; i < schedule.max_index; i++) {
            if(schedule.threads[i].state != FREE) {
                return 0;
            }
        }
    }
    return 1;
}
#endif