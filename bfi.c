#include <stdio.h>
#include <string.h>
#include "mem.h"
#include "stack.h"
#include "instructions.h"

#ifdef DEBUG
#define PRINT_PARAMS "'%c' (kood = %d)\n", c, c
#else
#define PRINT_PARAMS "%c", c
#endif

void interpret(struct stack_st *stack, char *program)
{
    int i = 0;
    while (program[i] != 0)
    {
        switch (program[i])
        {
        case BF_INCREASE:
            mem_inc();
            break;
        case BF_DECREASE:
            mem_dec();
            break;
        case BF_RIGHT:
            mem_right();
            break;
        case BF_LEFT:
            mem_left();
            break;
        case BF_DEBUG:
            mem_printDebug();
            break;
        case BF_READ:
        {
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
            break;
        }
        case BF_PRINT:
        {
            char c = mem_get();
            printf(PRINT_PARAMS);
            break;
        }
        case BF_START_LOOP:
        {
            if (mem_get() == 0)
            {
                int count = 1;
                i++;
                while (count != 0 && program[i] != 0)
                {
                    if (program[i] == BF_START_LOOP)
                    {
                        count++;
                    }
                    if (program[i] == BF_END_LOOP)
                    {
                        count--;
                    }
                    i++;
                };
                i--;
            }
            else
            {
                stack_push(stack, i);
            }
            break;
        }
        case BF_END_LOOP:
        {
            i = stack_pop(stack) - 1;
            break;
        }
        default:
            /* Ignoreerime sümboleid, mida me ei tunne. */
        }

        i++;
    }
}

/* Prindib välja terve käskude pinu */
void print_instruction_stack(struct BF_instruction_st **inst_arr, int inst_arr_len)
{
    printf("Instruction Stack (length = %d):\n", inst_arr_len);

    for (int i = 0; i < inst_arr_len; ++i)
    {
        struct BF_instruction_st *inst = inst_arr[i];
        if (!inst)
        {
            printf("[%03d] NULL\n", i);
            continue;
        }

        printf("[%03d] ", i);

        if (inst->run == BF_increment_run)
        {
            printf("INCREMENT delta=%d\n", inst->increment);
        }
        else if (inst->run == BF_move_run)
        {
            printf("MOVE positions=%d\n", inst->numberOfPositions);
        }
        else if (inst->run == BF_read_run)
        {
            printf("READ\n");
        }
        else if (inst->run == BF_write_run)
        {
            printf("WRITE value=%d\n", inst->writeValue);
        }
        else if (inst->run == BF_print_run)
        {
            printf("PRINT\n");
        }
        else if (inst->run == BF_printDebug_run)
        {
            printf("DEBUG\n");
        }
        else if (inst->run == BF_beginLoop_run)
        {
            printf("LOOP_START -> jump to %d if zero\n", inst->loopForwardIndex);
        }
        else if (inst->run == BF_endLoop_run)
        {
            printf("LOOP_END -> jump back to %d if nonzero\n", inst->loopBackIndex);
        }
        else
        {
            printf("UNKNOWN RUN FUNCTION\n");
        }
    }
}

void parse(struct BF_instruction_st **inst_arr, char *program)
{
    int i = 0;
    int inst_idx = 0;
    struct stack_st loopStack = EMPTY_STACK;

    while (program[i] != 0)
    {
        inst_arr[inst_idx] = NULL;

        switch (program[i])
        {

        case BF_INCREASE:
        case BF_DECREASE:
        {
            int delta = 0;
            while (program[i] == BF_INCREASE || program[i] == BF_DECREASE)
            {
                delta += (program[i] == BF_INCREASE) ? 1 : -1;
                i++;
            }
            inst_arr[inst_idx] = BF_increment_new(delta);
            inst_idx++;
            continue;
        }

        case BF_RIGHT:
        case BF_LEFT:
        {
            int move = 0;
            while (program[i] == BF_RIGHT || program[i] == BF_LEFT)
            {
                move += (program[i] == BF_RIGHT) ? 1 : -1;
                i++;
            }
            inst_arr[inst_idx] = BF_move_new(move);

            inst_idx++;
            continue;
        }

        case BF_READ:
            inst_arr[inst_idx] = BF_read_new();

            inst_idx++;
            break;

        case BF_PRINT:
            inst_arr[inst_idx] = BF_print_new();

            inst_idx++;
            break;

        case BF_DEBUG:
            inst_arr[inst_idx] = BF_printDebug_new();

            inst_idx++;
            break;

        case BF_START_LOOP:
            inst_arr[inst_idx] = BF_beginLoop_new();
            stack_push(&loopStack, inst_idx);

            inst_idx++;
            break;

        case BF_END_LOOP:
        {
            if (loopStack.len == 0)
            {
                fprintf(stderr, "Syntax error: unmatched ']'\n");
                exit(1);
            }
            int beginIndex = stack_pop(&loopStack);
            inst_arr[inst_idx] = BF_endLoop_new(beginIndex);
            inst_arr[beginIndex]->loopForwardIndex = inst_idx;

            inst_idx++;
            break;
        }

        default:
            // ignore unknown characters
            i++;
            continue;
        }

        i++;
    }

    if (loopStack.len != 0)
    {
        fprintf(stderr, "Syntax error: unmatched '['\n");
        exit(1);
    }
}

void run(struct BF_instruction_st **inst_arr, int inst_arr_len)
{
    int i = 0;
    while (1)
    {
        if (i < 0 || i >= inst_arr_len)
            break;
        if (inst_arr[i] != NULL)
        {
            inst_arr[i]->run(inst_arr[i], &i);
        }
        else
        {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }
}

void interpret2(char *program)
{
    /* Leiame programmi lähtekoodi pikkuse. */
    int program_len = strlen(program);

    /* Teeme massiivi, mis hoiab viitasid, mida on kokku program_len tükku. Viitade
       algväärtustamine toimub parse() funktsioonis. Massiivi pikkus on võetud varuga */
    struct BF_instruction_st **inst_arr = malloc(sizeof(struct BF_instruction_st *) * program_len);

    /* Parsime sisendprogrammi, mille tulemus salvestatakse inst_arr massiivi. */
    parse(inst_arr, program);

#ifdef DEBUG
    print_instruction_stack(inst_arr, strlen(program));
#endif

    /* Käivitame programmi. */
    run(inst_arr, program_len);

    int i = 0;
    while (1)
    {
        if (i < 0 || i >= program_len)
            break;
        if (inst_arr[i] != NULL)
        {
            free(inst_arr[i]);
            i++;
        }
        else
        {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }

    free(inst_arr);
}

void runPrintAsm(struct BF_instruction_st **inst_arr, int inst_arr_len)
{
    printf("global main\n");
    printf("extern printf\n");

    printf("section .data\n");
    printf("    bfmem: times 30000 db 0\n");
    printf("    fmt_char: db '%%c', 0\n");

    printf("section .text\n");
    printf("main:\n");

    printf("    push esi\n");
    printf("    lea esi, [bfmem]\n");

    int i = 0;
    while (1)
    {
        if (i < 0 || i >= inst_arr_len)
            break;
        if (inst_arr[i] != NULL)
        {
            inst_arr[i]->printAsm(inst_arr[i], &i);
        }
        else
        {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }

    printf("    pop esi\n");
    printf("    mov eax, 1     ; sys_exit\n");
    printf("    xor ebx, ebx   ; exit code 0\n");
    printf("    int 0x80\n");
    printf("    ret\n");
}

void interpret3(char *program)
{
    /* Leiame programmi lähtekoodi pikkuse. */
    int program_len = strlen(program);

    /* Teeme massiivi, mis hoiab viitasid, mida on kokku program_len tükku. Viitade
       algväärtustamine toimub parse() funktsioonis. Massiivi pikkus on võetud varuga */
    struct BF_instruction_st **inst_arr = malloc(sizeof(struct BF_instruction_st *) * program_len);

    /* Parsime sisendprogrammi, mille tulemus salvestatakse inst_arr massiivi. */
    parse(inst_arr, program);

    /* Käivitame programmi. */
    runPrintAsm(inst_arr, program_len);

    int i = 0;
    while (1)
    {
        if (i < 0 || i >= program_len)
            break;
        if (inst_arr[i] != NULL)
        {
            free(inst_arr[i]);
            i++;
        }
        else
        {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }

    free(inst_arr);
}

int main(int argc, char **argv)
{
    struct stack_st stack = {.len = 0, .size = 0, .arr = NULL};

    /* Käivitame programmi, mille kasutaja andis käsurealt. */

    if (argc > 2 && strcmp(argv[1], "--asm") == 0)
    {
        interpret3(argv[2]);
    }
    else
    {
        printf("1. programm - stack.c + mem.c\n");
        interpret(&stack, argv[1]);
        mem_init();
        printf("\n2. programm - mem.c + instructions.c\n");
        interpret2(argv[1]);
        printf("\n");
    }

    return 0;
}