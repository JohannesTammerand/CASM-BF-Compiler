#include <stdbool.h>

#ifndef MEM_H
#define MEM_H

#define MEM_SIZE 30000

struct mem_el {
    char val;
    bool init;
};

enum instructions_e {
    /* Liiguta mälu indeksit paremale. */
    BF_RIGHT      = '>',

    /* Liiguta mälu indeksit vasakule. */
    BF_LEFT       = '<',

    /* Suurenda mälu indeksil olevat väärtust. */
    BF_INCREASE   = '+',

    /* Vähenda mälu indeksil olevat väärtust. */
    BF_DECREASE   = '-',

    /* Loe kasutajalt sisendit ja salvesta see mäluindeksile. */
    BF_READ       = ',',

    /* Trüki välja mälu indeksil olev väärtus char'ina. */
    BF_PRINT      = '.',

    /* Tsükli algus. */
    BF_START_LOOP = '[',

    /* Tsükli lõpp. */
    BF_END_LOOP   = ']',

    /* Trüki silumise informatsioon. */
    BF_DEBUG      = '#'
};

void mem_init();

int mem_inc();

int mem_dec();

int mem_left();

int mem_right();

int mem_move(int numberOfPositions);

unsigned char mem_get();

int mem_add(int amount);

int mem_set(int v);

void mem_printDebug();

#endif