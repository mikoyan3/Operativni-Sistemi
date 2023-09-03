#include "../h/MemoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/TCB.hpp"
#include "../h/syscall_c.hpp"


void userMain();

int main(){
    __asm__ volatile("csrw stvec, %[vector]":: [vector] "r" ((uint64)(&Riscv::Table)|0x1));
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    //TCB* pocetna;
    //thread_create(&pocetna, nullptr, nullptr);
    TCB::running = new TCB(nullptr, nullptr);
    userMain();
    return 0;
}