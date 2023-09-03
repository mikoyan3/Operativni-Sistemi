//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../test/printing.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    ms_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::ecallHandler(){
    uint64 volatile spc;
    __asm__ volatile("csrr %0, sepc" : "=r"(spc));
    uint64 volatile sccause;
    __asm__ volatile("csrr %0, scause" : "=r"(sccause));
    if(sccause == 0x0000000000000008UL || sccause == 0x0000000000000009UL) {
        uint64 volatile cause;
        __asm__ volatile("mv %0, a0" : "=r" (cause));
        if (cause == 0x01) { //mem alloc
            size_t blokovi;
            __asm__ volatile("mv %0, a1" : "=r"(blokovi));
            void *pokazivac = memAllocator.mem_alloc(blokovi);
            __asm__ volatile("mv a0, %0" : : "r"(pokazivac));
        } else if (cause == 0x02) { //mem free
            void *pok;
            __asm__ volatile("mv %0, a1" : "=r"(pok));
            int rez = memAllocator.mem_free(pok);
            __asm__ volatile("mv a0, %0" : : "r"(rez));
        }
        else if (cause == 0x11) { // thread_create, a3 rutina, a2 argumenti, a1 rucka
            TCB* volatile handle;
            void* volatile arg;
            TCB::Body volatile start_routine;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            __asm__ volatile("mv %0, a2" : "=r"(arg));
            __asm__ volatile("mv %0, a6" : "=r"(start_routine));
            uint64* stck = (uint64*)memAllocator.mem_alloc((DEFAULT_STACK_SIZE*sizeof(uint64) + MemoryAllocator::getSize() + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);
            if(stck != nullptr) {
                TCB *novi = TCB::create(start_routine, arg, stck);
                uint64 *dest = reinterpret_cast<uint64 *>(handle);
                *dest = reinterpret_cast<uint64>(novi);
                __asm__ volatile("mv a0, %0" : : "r"(0));
            } else{
                __asm__ volatile("mv a0, %0" : : "r"(-1));
            }
        } else if (cause == 0x12) { // thread_exit
            TCB::exit();
        } else if (cause == 0x13) { // thread_dispatch
            TCB::dispatch();
        } else if (cause == 0x14) { // thread_join
            TCB* handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            handle->join();
        } else if (cause == 0x15){ // thread_create bez start, a3 rutina, a2 argumenti, a1 rucka
            TCB* volatile handle;
            void* volatile arg;
            TCB::Body volatile start_routine;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            __asm__ volatile("mv %0, a2" : "=r"(arg));
            __asm__ volatile("mv %0, a6" : "=r"(start_routine));
            uint64* stck = (uint64*)memAllocator.mem_alloc((DEFAULT_STACK_SIZE*sizeof(uint64) + MemoryAllocator::getSize() + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE); //(uint64*)__mem_alloc(DEFAULT_STACK_SIZE*sizeof(uint64));
            if(stck != nullptr) {
                TCB *novi = TCB::createWithoutStart(start_routine, arg, stck);
                uint64 *dest = reinterpret_cast<uint64 *>(handle);
                *dest = reinterpret_cast<uint64>(novi);
                __asm__ volatile("mv a0, %0" : : "r"(0));
            } else{
                __asm__ volatile("mv a0, %0" : : "r"(-1));
            }
        } else if (cause == 0x21){ // sem_open, a1 init, a2 handle
            sem_t* handle;
            unsigned init;
            __asm__ volatile("mv %0, a1" : "=r"(init));
            __asm__ volatile("mv %0, a2" : "=r"(handle));
            _sem *novi = _sem::create(init);
            uint64 *dest = reinterpret_cast<uint64 *>(handle);
            *dest = reinterpret_cast<uint64>(novi);
            __asm__ volatile("mv a0, %0" : : "r"(0));
        } else if (cause == 0x22){ // sem_close, a1 handle
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            int volatile pov = _sem::close(handle);
            __asm__ volatile("mv a0, %0" : : "r"(pov));
        } else if (cause == 0x23){ // sem_wait, a1 id
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            int volatile pov = _sem::wait(handle);
            __asm__ volatile("mv a0, %0" : : "r"(pov));
        } else if (cause == 0x24){ // sem_signal, a1 id
            sem_t handle;
            __asm__ volatile("mv %0, a1" : "=r"(handle));
            int volatile pov = _sem::signal(handle);
            __asm__ volatile("mv a0, %0" : : "r"(pov));
        }
        spc = spc + 0x04;
        __asm__ volatile("csrw sepc, %0" : : "r"(spc));
    } else{
        printInt(sccause);
        printString("\n");
        printInt(spc);
        printString("\n");
    }
}

void Riscv::timerHandler() {
    mc_sip(SIP_SSIP);
}

void Riscv::consoleHandler() {
    console_handler();
}

