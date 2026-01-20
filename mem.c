#include <stdio.h>
#include "mem.h"

int idx = 0;
struct mem_el mem[30000];

void mem_init() {
    for (int i = 0; i < MEM_SIZE; i++) {
        mem[i].val = 0;
        mem[i].init = 0;
    }
    idx = 0;
}

unsigned char mem_get() {
    return mem[idx].val;
}

int mem_set(int v) {
    mem[idx].val = (unsigned char)v; // ensure wrap 0..255
    mem[idx].init = 1;
    return mem[idx].val;
}

int mem_add(int amount) {
    mem[idx].val = (unsigned char)(mem[idx].val + amount); // wrap correctly
    mem[idx].init = 1;
    return mem[idx].val;
}

int mem_inc() { return mem_add(1); }
int mem_dec() { return mem_add(-1); }

int mem_move(int n) {
    idx += n;
    if (idx >= MEM_SIZE) idx %= MEM_SIZE;
    if (idx < 0) idx = MEM_SIZE + (idx % MEM_SIZE); // wrap negative
    return idx;
}

int mem_left() { return mem_move(-1); }
int mem_right() { return mem_move(1); }

void mem_printDebug() {
    int start = idx;
    printf("idx=%d: ", idx);
    for (int i = 0; i < 10; i++) {
        printf("%d ", mem[idx].val);
        mem_right();
    }
    printf("\n");
    idx = start;
}