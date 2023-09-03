//
// Created by os on 8/7/23.
//

#include "../h/TCB.hpp"

TCB *TCB::running = nullptr;
TCB *TCB::mainNit = nullptr;

//TCB::TCB(TCB::Body body, void* arg){
//    this->body = body;
//   this->arg = arg;
//    stanje = Stanje::INITIALIZING;
//   if(body != nullptr){
//       stack = new uint64[DEFAULT_STACK_SIZE];
//   } else {
//       stack = nullptr;
//  }
//   context.ra = (uint64)&threadWrapper;
//  context.sp = (uint64)&stack[DEFAULT_STACK_SIZE];
//   if(body == nullptr){
//     stanje = Stanje::MAIN;
//   }
//}
TCB::TCB(TCB::Body body, void* arg) {
    TCB::mainNit = this;
}

TCB *TCB::createWithoutStart(TCB::Body body, void* arg, uint64* stck){
    TCB* nova = new TCB();
    nova->body = body;
    nova->arg = arg;
    nova->stanje = Stanje::INITIALIZING;
    if (body != nullptr) nova->stack = stck;
    else nova->stack = nullptr;
    nova->context.ra = (uint64)&threadWrapper;
    nova->context.sp = (uint64)&stck[DEFAULT_STACK_SIZE];
    if(body == nullptr){
        TCB::mainNit = nova;
        nova->stanje = Stanje::MAIN;
    }
    //nova->start();
    return nova;
}

TCB *TCB::create(TCB::Body body, void* arg, uint64* stck){
    TCB* nova = new TCB();
    nova->body = body;
    nova->arg = arg;
    nova->stanje = Stanje::INITIALIZING;
    //nova->blocked = (TCB*)__mem_alloc(sizeof(TCB));
    //nova->flag = false;
    //uint64* stck = (uint64*)__mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));
    if (body != nullptr) nova->stack = stck;
    else nova->stack = nullptr;
    nova->context.ra = (uint64)&threadWrapper;
    nova->context.sp = (uint64)&stck[DEFAULT_STACK_SIZE];
    if(body == nullptr){
        TCB::mainNit = nova;
        nova->stanje = Stanje::MAIN;
    }
    nova->start();
    return nova;
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    if(running->stanje == Stanje::READY){
        running->body(running->arg);
    }
    running->stanje = Stanje::FINISHED;
    TCB* tek = running->suspended.removeFirst();
    while(tek != nullptr){
        tek->stanje = Stanje::READY;
        Scheduler::put(tek);
        tek = running->suspended.removeFirst();
    }
    thread_dispatch();
}

int TCB::start(){
    if(stanje == Stanje::INITIALIZING || stanje == Stanje::SUSPENDED){
        stanje = Stanje::READY;
        Scheduler::put(this);
        return 0;
    } else if (stanje == Stanje::READY) return 0;
    else return -1;
}

void TCB::dispatch(){
    TCB* old = running;
    if(old->stanje != Stanje::SUSPENDED && old->stanje != Stanje::FINISHED && old->stanje != Stanje::MAIN){
        Scheduler::put(old);
        old->stanje = Stanje::READY;
    }
    if(Scheduler::getPopunjenost() != 0) {
        running = Scheduler::get();
        TCB::contextSwitch(&old->context, &running->context);
    } else {
        running = TCB::mainNit;
        TCB::contextSwitch(&old->context, &running->context);
    }
}

void TCB::exit(){
    running->stanje = Stanje::FINISHED;
    TCB* tek = running->suspended.removeFirst();
    while(tek != nullptr){
        tek->stanje = Stanje::READY;
        Scheduler::put(tek);
        tek = running->suspended.removeFirst();
    }
    TCB::dispatch();
}


void TCB::join(){
    TCB* run = TCB::running;
    run->stanje = Stanje::SUSPENDED;
    this->suspended.addLast(run);
    TCB::dispatch();
}



