//
// Created by os on 8/7/23.
//
#include "../h/syscall_cpp.hpp"


void* operator new(size_t size){
    return mem_alloc(size); //return __mem_alloc(size);
}
void operator delete(void* addr){ mem_free(addr); }//__mem_free(addr); }

void operator delete[](void* addr) { mem_free(addr); }//__mem_free(addr);}
void* operator new[](size_t size){ return mem_alloc(size); }//return __mem_alloc(size); }


void Thread::omotac(void* thread) {
    ((Thread*)thread)->run();
}

Thread::Thread(void (*body)(void *), void *arg) {
    this->arg = arg;
    thread_createCPP(&myHandle, body, arg);
}

Thread::Thread(){
    this->arg = this;
    thread_createCPP(&myHandle, Thread::omotac, this);
}
Thread::~Thread(){
    delete myHandle;
}

int Thread::start(){
    return myHandle->start();
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return 0;
}

void Thread::join(){
    thread_join(myHandle);
}

Semaphore::Semaphore (unsigned init){
    sem_open(&myHandle, init);
}
Semaphore::~Semaphore (){
    sem_close(myHandle);
    myHandle = nullptr;
}
int Semaphore::wait (){
    int pov = sem_wait(myHandle);
    return pov;
}
int Semaphore::signal (){
    int pov = sem_signal(myHandle);
    return pov;
}