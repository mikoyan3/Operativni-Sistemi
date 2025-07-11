//
// Created by os on 8/9/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_HPP
#define PROJECT_BASE_V1_1_SYSCALL_C_HPP
#include "../lib/hw.h"
#include "../h/TCB.hpp"
#include "../lib/console.h"
#include "../h/_sem.hpp"
#include "../h/MemoryAllocator.hpp"


class TCB;
typedef TCB* thread_t;

void* mem_alloc (size_t size);
int mem_free (void* pok);

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_createCPP(thread_t* handle, void(*start_routine)(void*), void* arg);
int thread_exit ();
void thread_dispatch ();
void thread_join(thread_t handle);

char getc();
void putc(char c);

class _sem;
typedef _sem* sem_t;
int sem_open (sem_t* handle, unsigned init);
int sem_close (sem_t handle);
int sem_wait (sem_t id);
int sem_signal (sem_t id);

typedef unsigned long time_t;
int time_sleep (time_t);


#endif //PROJECT_BASE_V1_1_SYSCALL_C_HPP
