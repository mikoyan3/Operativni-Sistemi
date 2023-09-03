//
// Created by os on 8/8/23.
//

#include "../h/list.hpp"

void List::addFirst(TCB *data) {
    Elem *elem = (Elem*)memAllocator.mem_alloc(sizeof(Elem));//(Elem*)__mem_alloc(sizeof(Elem));//new Elem(data, head);
    elem->data = data;
    elem->next = head;
    head = elem;
    if (!tail) { tail = head; }
}

void List::addLast(TCB *data){
    Elem *elem = (Elem*)memAllocator.mem_alloc(sizeof(Elem));//(Elem*)__mem_alloc(sizeof(Elem));//new Elem(data, 0);
    elem->data = data;
    elem->next = nullptr;
    if (tail)
    {
        tail->next = elem;
        tail = elem;
    } else
    {
        head = tail = elem;
    }
}

TCB* List::removeFirst(){
    if (!head) { return 0; }

    Elem *elem = head;
    head = head->next;
    if (!head) { tail = 0; }

    TCB *ret = elem->data;
    memAllocator.mem_free(elem);//__mem_free(elem);//delete elem;
    return ret;
}

TCB* List::removeLast(){
    if (!head) { return 0; }

    Elem *prev = 0;
    for (Elem *curr = head; curr && curr != tail; curr = curr->next)
    {
        prev = curr;
    }

    Elem *elem = tail;
    if (prev) { prev->next = 0; }
    else { head = 0; }
    tail = prev;

    TCB *ret = elem->data;
    memAllocator.mem_free(elem);//__mem_free(elem);//delete elem;
    return ret;
}