#include <stdio.h>
#include "mem.h"

int idx = 0;
struct mem_el mem[30000];

void mem_init() {
    for (idx = 29999; idx >= 0; idx--) {
        mem[idx].init = 0;
        mem[idx].val = 0;
    }
}

int mem_add(int amount) {
    if (mem[idx].init) {
        mem[idx].val = mem[idx].val + amount;
    } else {
        mem[idx].val = amount;
        mem[idx].init = 1;
    }

    return mem[idx].val;
}

int mem_inc() {
    return mem_add(1);
}

int mem_dec() {
    return mem_add(-1);
}

int mem_move(int numberOfPositions) {
    idx = idx + numberOfPositions;
    if (idx > 29999) {
        idx = idx - 29999;
    }
    if (idx < 0) {
        idx = 29999 + idx;
    }
    
    return idx;
}

int mem_left() {
    mem_move(-1);
}

int mem_right() {
    mem_move(1);
}

int mem_set(int v) {
    mem[idx].val = v;
    mem[idx].init = 1;
    return v;
}

int mem_get() {
    if (mem[idx].init) {
        return mem[idx].val;
    } else {
        return 0;
    }
}

void mem_printDebug() {
    int endIdx;
    if (idx < 28889) {
        endIdx = idx + 9;
    } else {
        endIdx = idx - 28889 + 9;
    }

    printf("index: %d mem[%d .. %d]:", idx, idx, endIdx);
    
    for (int i = 0; i < 10; i++) {
        if (mem[idx].init) {
            printf(" %d", mem_get());
        } else {
            printf(" 0");
        }
        mem_right();
    }
    printf("\n");

    for (int i = 0; i < 10; i++) {
        mem_left();
    }
}