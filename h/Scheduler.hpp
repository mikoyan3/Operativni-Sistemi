//
// Created by os on 8/15/22.
//

#ifndef PROJECT_BASE_V1_1_SCHEDULER_HPP
#define PROJECT_BASE_V1_1_SCHEDULER_HPP
#include "../h/list.hpp"
class TCB;

class Scheduler {
private:
    static List readyThreadQueue;

public:
    static TCB *get();
    static int popunjenost;
    static void put(TCB *tcb);
    static int getPopunjenost();
};


#endif //PROJECT_BASE_V1_1_SCHEDULER_HPP
