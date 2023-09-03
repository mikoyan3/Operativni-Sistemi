//
// Created by os on 6/27/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#include "../lib/console.h"
#include "../lib/hw.h"
//#include "../h/printing.hpp"



class MemoryAllocator {
private:
    struct Segment{
        size_t pocetak;
        size_t kraj;
        Segment* sled;
    };
    Segment *prviZauzet;
    Segment *prviSlobodan;
    bool instance = false;
public:
    void *mem_alloc(size_t size);
    int mem_free(void* ptr);
    void dodajZauzet(Segment *s);
    void dodajSlobodan(Segment *s);
    static size_t getSize() { return sizeof(Segment); }
};

extern MemoryAllocator memAllocator;


#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
