#include <stdio.h>
#include <stdlib.h>

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

struct BF_instruction_st {
    void (*run)(struct BF_instruction_st *instruction, int *index);

    union {
        int increment;
        int numberOfPositions;
        int writeValue;
        int loopForwardIndex;
        int loopBackIndex;
    };
};

struct BF_instruction_st *BF_increment_new(int increment);

struct BF_instruction_st *BF_move_new(int numberOfPositions);

struct BF_instruction_st *BF_read_new();

struct BF_instruction_st *BF_write_new(int writeValue);

struct BF_instruction_st *BF_beginLoop_new();

struct BF_instruction_st *BF_endLoop_new(int loopBackIndex);

struct BF_instruction_st *BF_print_new();

struct BF_instruction_st *BF_printDebug_new();

void BF_increment_run(struct BF_instruction_st *instruction, int *index);

void BF_move_run(struct BF_instruction_st *instruction, int *index);

void BF_read_run(struct BF_instruction_st *instruction, int *index);

void BF_write_run(struct BF_instruction_st *instruction, int *index);

void BF_beginLoop_run(struct BF_instruction_st *instruction, int *index);

void BF_endLoop_run(struct BF_instruction_st *instruction, int *index);

void BF_print_run(struct BF_instruction_st *instruction, int *index);

void BF_printDebug_run(struct BF_instruction_st *instruction, int *index);

#endif