//
// Created by os on 8/7/23.
//

#ifndef PROJECT_BASE_V1_1__SEM_HPP
#define PROJECT_BASE_V1_1__SEM_HPP
#include "../lib/hw.h"
#include "../h/syscall_c.hpp"
#include "../lib/mem.h"
#include "../h/Scheduler.hpp"



class _sem {
public:
    _sem(){}
    static _sem* create(unsigned init);
    int getVrednost() const{
        return vrednostSemafora;
    }
    static int close(_sem *handle);
    static int wait(_sem *handle);
    void block();
    void unblock();
    void unblockAll();
    static int signal(_sem *handle);
private:
    struct Elem{
        TCB* data;
        Elem* next = nullptr;
    };
    Elem* head;
    bool closed;
    bool dealocated;
    int vrednostSemafora;
    friend class TCB;
};


#endif //PROJECT_BASE_V1_1__SEM_HPP
