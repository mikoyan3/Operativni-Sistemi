//
// Created by os on 6/27/23.
//

#include "../h/MemoryAllocator.hpp"

MemoryAllocator memAllocator;
void* MemoryAllocator::mem_alloc(size_t size){
    if(memAllocator.instance == false){ //prva inicijalizacija
        prviSlobodan = (Segment*)(HEAP_START_ADDR);
        prviZauzet = prviSlobodan + 1;
        Segment *prvi = prviZauzet + 1;
        prvi->pocetak = (size_t)prvi + sizeof(Segment);
        prvi->kraj = (size_t)HEAP_END_ADDR;
        prvi->sled = nullptr;
        prviSlobodan->sled = prvi;
        memAllocator.instance = true;
    }
    size_t potrebno = size * MEM_BLOCK_SIZE;
    Segment *tek = prviSlobodan->sled;
    Segment *pret = nullptr;
    while(tek != nullptr && (tek->kraj - tek->pocetak) < potrebno) { //trazenje dovoljno velikog segmenta
        pret = tek;
        tek = tek->sled;
    }
    if(tek == nullptr) return nullptr; //ne postoji dovoljno veliki segment
    if(potrebno == (tek->kraj - tek->pocetak)){ //ako je potrebno tacno koliko je segment veliki
        if(pret == nullptr){ //ako je prvi u listi
            prviSlobodan->sled = tek->sled;
            tek->sled = nullptr;
            dodajZauzet(tek);
            return (void*)tek->pocetak;
        }else{ //ako nije prvi u listi
            pret->sled = tek->sled;
            tek->sled = nullptr;
            dodajZauzet(tek);
            return (void*)tek->pocetak;
        }
    }else{ //ako je potreban "prelom"
        size_t kraj = tek->kraj;
        Segment* sled = tek->sled;
        Segment* novi = (Segment*)tek->pocetak;
        novi->pocetak = (size_t)novi + sizeof(Segment);
        novi->kraj = novi->pocetak + potrebno - sizeof(Segment);
        novi->sled = nullptr;
        tek->pocetak = novi->kraj + 1;
        tek->kraj = kraj;
        tek->sled = sled;
        if(pret != nullptr) pret->sled = tek;
        else prviSlobodan->sled = tek;
        dodajZauzet(novi);
        return (void*)(novi->pocetak);
    }
}

void MemoryAllocator::dodajZauzet(MemoryAllocator::Segment *s) {
    if(prviZauzet->sled == nullptr) prviZauzet->sled = s; //ako ne postoji ni jedan zauzet
    else{ //dodavanje na pocetak
        s->sled = prviZauzet->sled;
        prviZauzet->sled = s;
    }
}

int MemoryAllocator::mem_free(void *ptr){
    if((size_t)(ptr) < (size_t)(HEAP_START_ADDR) || (size_t)(ptr) > (size_t)(HEAP_END_ADDR)) return -1;
    Segment *tek = prviZauzet->sled;
    Segment *pret = nullptr;
    while(tek!=nullptr && tek->pocetak != 0 && tek->pocetak != (size_t)ptr) { //potrega u listi zauzetih
        pret = tek;
        tek = tek->sled;
    }
    if(tek == nullptr) return -2; //ako nije predjasnje alociran
    if(pret == nullptr){ //ako je prvi u listi
        prviZauzet->sled = tek->sled;
        tek->sled = nullptr;
        dodajSlobodan(tek);
        return 0;
    }else{ //ako nije prvi u listi
        pret->sled = tek->sled;
        tek->sled = nullptr;
        dodajSlobodan(tek);
        return 0;
    }
}

void MemoryAllocator::dodajSlobodan(MemoryAllocator::Segment *s) { //dodavanje Sortirano
    Segment* tek = prviSlobodan->sled;
    Segment* pret = nullptr;
    if(tek == nullptr) prviSlobodan->sled = s;//ako ne postoji ni jedan u listi
    else{
        while(tek != nullptr && tek->pocetak < s->pocetak) { //trazenje mesta za umetanje
            pret = tek;
            tek = tek->sled;
        }
        if(tek == nullptr){ //ako treba da se umetne kao poslednji, ako su jedan za drugim spajanje
            if(pret->kraj + sizeof(Segment) + 1 == s->pocetak || pret->kraj + 1 == s->pocetak) pret->kraj = s->kraj;
            else pret->sled = s; //ako ne treba spajanje
            return;
        }
        if(pret != nullptr){ //ako je "u sred" liste
            if(pret->kraj + sizeof(Segment) + 1 == s->pocetak || pret->kraj + 1 == s->pocetak){ //ako prethodni treba da se spoji
                if(s->kraj + sizeof(Segment) + 1 == tek->pocetak || s->kraj + 1 == tek->pocetak){ //ako i sledeci treba da se spoji
                    pret->sled = tek->sled;
                    pret->kraj = tek->kraj;
                } else { //ako sledeci ne treba da se spoji
                    pret->kraj = s->kraj;
                }
            } else if(s->kraj + sizeof(Segment) + 1 == tek->pocetak || s->kraj + 1 == tek->pocetak){ //ako samo sledeci treba da se spoji
                s->sled = tek->sled;
                pret->sled = s;
                s->kraj = tek->kraj;
            } else { // ako nista ne treba da se spaja
                pret->sled = s;
                s->sled = tek;
            }
        }else{ //ako je na pocetku liste
            if(s->kraj + sizeof(Segment) + 1 == tek->pocetak || s->kraj + 1 == tek->pocetak){ //ako sledeci treba da se spoji
                s->kraj = tek->kraj;
                s->sled = tek->sled;
                prviSlobodan->sled = s;
            }else{ //ako ne treba da se spajaju
                s->sled = tek;
                prviSlobodan->sled = s;
            }
        }
    }
}
