#include <stdio.h>

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

struct BF_instruction_st {
    void (*run)(struct BF_instruction_st *instruction, int *index);

    union {
        int increment;
        int numberOfPositions;
        int loopForwardIndex;
        int loopBackIndex;
    };
};

struct BF_instruction_st *BF_increment_new (int increment) {
    struct BF_instruction_st *new = NULL;

    if (increment == 0) {
        printf("Liitmisinstruktsiooni parameeter ei saa olla 0!");

    }
}

#endif