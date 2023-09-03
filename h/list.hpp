//
// Created by marko on 20.4.22..
//

#ifndef PROJECT_BASE_V1_1_LIST_HPP
#define PROJECT_BASE_V1_1_LIST_HPP

#include "../h/MemoryAllocator.hpp"
#include "../lib/mem.h"
//#include "../h/TCB.hpp"

class TCB;

class List
{
private:
    struct Elem
    {
        TCB *data;
        Elem *next;

        Elem(TCB *data, Elem *next) : data(data), next(next) {}

    };

    Elem *head, *tail;

public:
    List() : head(0), tail(0) {}

    List(const List &other) = delete;

    List &operator=(const List &) = delete;

    void addFirst(TCB *data);

    void addLast(TCB *data);

    TCB *removeFirst();

    TCB *removeLast();

};

#endif //PROJECT_BASE_V1_1_LIST_HPP
