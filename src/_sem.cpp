//
// Created by os on 8/7/23.
//

#include "../h/_sem.hpp"


int _sem::close(_sem* handle){
    if(handle == nullptr) return -1;
    if(handle->closed) return -1;
    handle->unblockAll();
    handle->closed = true;
    memAllocator.mem_free(handle);
    handle = nullptr;
    return 0;
}

int _sem::wait(_sem *handle){
    if(handle == nullptr) return -1;
    if(handle->closed) return -1;
    handle->vrednostSemafora--;
    if(handle->vrednostSemafora < 0) handle->block();
    return 0;
}

int _sem::signal(_sem *handle){
    if(handle == nullptr) return -1;
    if(handle->closed) return -1;
    handle->vrednostSemafora++;
    if(handle->vrednostSemafora <= 0) handle->unblock();
    return 0;
}

_sem* _sem::create(unsigned init){
    _sem *novi = (_sem*)memAllocator.mem_alloc((sizeof(_sem*) + MemoryAllocator::getSize() + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);//(_sem*)__mem_alloc(sizeof(_sem));//new _sem();
    novi->closed = false;
    novi->vrednostSemafora = init;
    novi->dealocated = false;
    novi->head = nullptr;
    return novi;
}

void _sem::block(){
    Elem* novi = (Elem*)memAllocator.mem_alloc((sizeof(Elem*) + MemoryAllocator::getSize() + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE);//(Elem*)__mem_alloc(sizeof(Elem));
    novi->data = TCB::running;
    novi->next = nullptr;
    TCB::running->posSuspended(true);
    if(head == nullptr){
        head = novi;
    } else {
        Elem* tek = head;
        while(tek->next != nullptr) tek = tek->next;
        tek->next = novi;
    }
    TCB::dispatch();
}

void _sem::unblock(){
    if(head != nullptr){
        Elem* tek = head;
        head = head->next;
        tek->data->posSuspended(false);
        Scheduler::put(tek->data);
    }
}

void _sem::unblockAll(){
    Elem* tek = head;
    while(tek){
        tek->data->posSuspended(false);
        Scheduler::put(tek->data);
        tek = tek->next;
    }
}