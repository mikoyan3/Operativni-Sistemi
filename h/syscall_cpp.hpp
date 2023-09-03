//
// Created by os on 8/7/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#include "syscall_c.hpp"

void* operator new(size_t);
void operator delete(void*);


class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}
private:
    thread_t myHandle;
    static void omotac(void*);
    void* arg;
};

class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
protected:
    PeriodicThread (time_t period){}
    virtual void periodicActivation () {}
};

class Console {
public:
    static char getc (){ return 'c';}
    static void putc (char){}
};

void operator delete[](void* addr);
void* operator new[](size_t size);

#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
