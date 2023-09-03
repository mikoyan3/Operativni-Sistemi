//
// Created by os on 8/9/22.
//

#include "../h/syscall_c.hpp"

class TCB;
typedef TCB* thread_t;

void* mem_alloc (size_t size){
    size_t blokovi = (size + sizeof(MemoryAllocator::getSize()) + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE;
    __asm__ volatile("mv a1, %0" : : "r"(blokovi));
    __asm__ volatile("mv a0, %0" : : "r"(0x01));
    __asm__ volatile("ecall");
    void* povratnaVrednost;
    __asm__ volatile("mv %0, a0" : "=r"(povratnaVrednost));
    return povratnaVrednost;
}

int mem_free (void* pok){
    __asm__ volatile("mv a1, %0" : : "r"(pok));
    __asm__ volatile("mv a0, %0" : : "r"(0x02));
    __asm__ volatile("ecall");
    int povratnaVrednost;
    __asm__ volatile("mv %0, a0" : "=r"(povratnaVrednost));
    return povratnaVrednost;
}

int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a6, %0" : : "r"(start_routine));
    __asm__ volatile("mv a2, %0" : : "r"(arg));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x11));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int thread_createCPP(thread_t* handle, void(*start_routine)(void*), void* arg){
    __asm__ volatile("mv a6, %0" : : "r"(start_routine));
    __asm__ volatile("mv a2, %0" : : "r"(arg));
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x15));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int thread_exit (){
    __asm__ volatile("mv a0, %0" : : "r"(0x12));
    __asm__ volatile("ecall");
    return 0;
}


void thread_dispatch (){
    __asm__ volatile("mv a0, %0" : : "r"(0x13));
    __asm__ volatile("ecall");
}

void thread_join(thread_t handle){
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x14));
    __asm__ volatile("ecall");
}

char getc(){
    return __getc();
}

void putc(char c){
    __putc(c);
}

int sem_open (sem_t* handle, unsigned init){
    __asm__ volatile("mv a2, %0" : : "r"(handle));
    __asm__ volatile("mv a1, %0" : : "r"(init));
    __asm__ volatile("mv a0, %0" : : "r"(0x21));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_close (sem_t handle){
    __asm__ volatile("mv a1, %0" : : "r"(handle));
    __asm__ volatile("mv a0, %0" : : "r"(0x22));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_wait (sem_t id){
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x23));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}
int sem_signal (sem_t id){
    __asm__ volatile("mv a1, %0" : : "r"(id));
    __asm__ volatile("mv a0, %0" : : "r"(0x24));
    __asm__ volatile("ecall");
    volatile int returnValue;
    __asm__ volatile("mv %0, a0" : "=r"(returnValue));
    return returnValue;
}

int time_sleep (time_t){ return 1; }



