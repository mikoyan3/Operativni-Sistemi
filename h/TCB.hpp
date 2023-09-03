//
// Created by os on 8/7/23.
//

#ifndef PROJECT_BASE_V1_1_TCB_HPP
#define PROJECT_BASE_V1_1_TCB_HPP

#include "../lib/hw.h"
#include "../h/Scheduler.hpp"
#include "../lib/mem.h"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

class List;

class TCB {
public:
    using Body = void(*)(void*);
    TCB(Body body, void* arg);
    TCB(){}
    ~TCB() { delete[] stack; }//memAllocator.mem_free(stack); }//__mem_free(stack); }
    int start();
    static void exit();
    static void dispatch();
    void join();
    static TCB* running;
    static TCB* mainNit;
    static TCB* create(Body body, void* arg, uint64* stck);
    static TCB* createWithoutStart(Body body, void* arg, uint64* stck);
    bool stackNull(){
        if(stack == nullptr) return true;
        else return false;
    }
    void setBody(Body body){
        this->body = body;
    }
    void setArg(void* arg){
        this->arg = arg;
    }
    void posSuspended(bool flag){
        if(flag) this->stanje = Stanje::SUSPENDED;
        else this->stanje = Stanje::READY;
    }
private:
    enum Stanje{
        INITIALIZING,
        READY,
        SUSPENDED,
        FINISHED,
        MAIN
    };

    List suspended;
    Stanje stanje;
    uint64* stack;
    Body body;
    void* arg;
    struct Context{
        uint64 ra;
        uint64 sp;
    };
    Context context;
    static void threadWrapper();
    static void contextSwitch(Context *stari, Context* novi);
};


#endif //PROJECT_BASE_V1_1_TCB_HPP
