#include <stdlib.h>
#include "instructions.h"
#include "mem.h"

#ifdef DEBUG
  #define INST_PRINT_PARAMS "'%c' (kood = %d)\n", c, c
#else
  #define INST_PRINT_PARAMS "%c", c
#endif

struct BF_instruction_st *BF_increment_new (int increment) {
    struct BF_instruction_st *new = NULL;

    if (increment == 0) {
        printf("Liitmisinstruktsiooni parameeter ei saa olla 0!");

        goto cleanup;

    }

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->increment = increment;
    new->run = BF_increment_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_move_new (int numberOfPositions) {
    struct BF_instruction_st *new = NULL;

    if (numberOfPositions == 0) {
        printf("Liigutamisinstruktsiooni parameeter ei saa olla 0!");

        goto cleanup;

    }

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->numberOfPositions = numberOfPositions;
    new->run = BF_move_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_read_new () {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->run = BF_read_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_write_new (int writeValue) {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->writeValue = writeValue;
    new->run = BF_write_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_beginLoop_new() {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }


    new->loopForwardIndex = -1;
    new->run = BF_beginLoop_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_endLoop_new(int loopBackIndex) {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->loopBackIndex = loopBackIndex;
    new->run = BF_endLoop_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_print_new() {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->run = BF_print_run;

    cleanup:
        return new;
}

struct BF_instruction_st *BF_printDebug_new() {
    struct BF_instruction_st *new = NULL;

    new = malloc(sizeof(struct BF_instruction_st));
    if (new == NULL) {
        printf("Mälu küsimine ebaõnnestus.");
        goto cleanup;
    }

    new->run = BF_printDebug_run;

    cleanup:
        return new;
}

void BF_increment_run(struct BF_instruction_st *instruction, int *index) {
    mem_add(instruction->increment);

    ++*index;
}

void BF_move_run(struct BF_instruction_st *instruction, int *index) {
    mem_move(instruction->numberOfPositions);

    ++*index;
}

void BF_read_run(struct BF_instruction_st *instruction, int *index) {
    /* Loeme märgi standardsisendist (kasutaja sisestab konsooli). */
    int c = getc(stdin);
    if (EOF == c)
    {
        /* Sisendi lõpu korral lõpetame interpretaatori töö. */
        printf("Sisendi lõpp!\n");
        return;
    }

    /* Lisame mällu loetud väärtuse. */
    mem_set(c);
}

void BF_write_run(struct BF_instruction_st *instruction, int *index) {
    mem_set(instruction->writeValue);

    ++*index;
}

void BF_beginLoop_run(struct BF_instruction_st *instruction, int *index) {
    int val = mem_get();

    if (instruction->loopForwardIndex < 0) {
        printf("Tsükli algus algväärtustamata!\n");
        return;
    }

    if (val == 0) {
        *index = instruction->loopForwardIndex + 1;
    } else {
        ++*index;
    }
}

void BF_endLoop_run(struct BF_instruction_st *instruction, int *index) {
    int val = mem_get();

    if (instruction->loopBackIndex < 0) {
        printf("Tsükli lõpp algväärtustamata!\n");
        return;
    }

    if (val == 0) {
        ++*index;
    } else {
        *index = instruction->loopBackIndex;
    }
}

void BF_print_run(struct BF_instruction_st *instruction, int *index) {
    char c = mem_get();
    printf(INST_PRINT_PARAMS);

    ++*index;
}

void BF_printDebug_run(struct BF_instruction_st *instruction, int *index) {
    mem_printDebug();

    ++*index;
}