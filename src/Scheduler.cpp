//
// Created by os on 8/15/22.
//

#include "../h/Scheduler.hpp"

List Scheduler::readyThreadQueue;
int Scheduler::popunjenost = 0;
TCB *Scheduler::get()
{
    popunjenost--;
    return readyThreadQueue.removeFirst();
}
int Scheduler::getPopunjenost() { return popunjenost; }
void Scheduler::put(TCB *tcb)
{
    readyThreadQueue.addLast(tcb);
    popunjenost++;
}