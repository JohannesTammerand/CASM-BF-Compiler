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
        case BF_START_LOOP: {
            if (mem_get() == 0) {
                int count = 1;
                i++;
                while (count != 0 && program[i] != 0) {
                    if (program[i] == BF_START_LOOP) {
                        count++;
                    }
                    if (program[i] == BF_END_LOOP) {
                        count--;
                    }
                    i++;
                };
                i--;
            } else {
                stack_push(stack, i);
            }
            break;
        }
        case BF_END_LOOP: {
            i = stack_pop(stack) - 1;
            break;
        }
        default:
            /* Ignoreerime sümboleid, mida me ei tunne. */
        }

        i++;
    }
}

void parse(struct BF_instruction_st **inst_arr, char *program) {
    int i = 0;
    struct stack_st loopStack = EMPTY_STACK;

    while (program[i] != 0) {
        inst_arr[i] = NULL;

        switch (program[i]) {

            case BF_INCREASE:
            case BF_DECREASE: {
                int delta = 0;
                while (program[i] == BF_INCREASE || program[i] == BF_DECREASE) {
                    delta += (program[i] == BF_INCREASE) ? 1 : -1;
                    i++;
                }
                inst_arr[i - 1] = BF_increment_new(delta);
                continue;
            }

            case BF_RIGHT:
            case BF_LEFT: {
                int move = 0;
                while (program[i] == BF_RIGHT || program[i] == BF_LEFT) {
                    move += (program[i] == BF_RIGHT) ? 1 : -1;
                    i++;
                }
                inst_arr[i - 1] = BF_move_new(move);
                continue;
            }

            case BF_READ:
                inst_arr[i] = BF_read_new();
                break;

            case BF_PRINT:
                inst_arr[i] = BF_print_new();
                break;

            case BF_DEBUG:
                inst_arr[i] = BF_printDebug_new();
                break;

            case BF_START_LOOP:
                inst_arr[i] = BF_beginLoop_new();
                stack_push(&loopStack, i);
                break;

            case BF_END_LOOP: {
                int beginIndex = stack_pop(&loopStack);
                inst_arr[i] = BF_endLoop_new(beginIndex);
                inst_arr[beginIndex]->loopForwardIndex = i;
                break;
            }

            default:
                break;
        }
        i++;
    }
}

void run(struct BF_instruction_st **inst_arr, int inst_arr_len) {
    int i = 0;
    while (1) {
        if (i < 0 || i >= inst_arr_len) break;
        if (inst_arr[i] != NULL) {
            inst_arr[i]->run(inst_arr[i], &i);
        } else {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }
}

void runPrintAsm(struct BF_instruction_st **inst_arr, int inst_arr_len) {
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
    while (1) {
        if (i < 0 || i >= inst_arr_len) break;
        if (inst_arr[i] != NULL) {
            inst_arr[i]->printAsm(inst_arr[i], &i);
        } else {
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

void interpret2(char *program) {
    /* Leiame programmi lähtekoodi pikkuse. */
    int program_len = strlen(program);
    
    /* Teeme massiivi, mis hoiab viitasid, mida on kokku program_len tükku. Viitade
       algväärtustamine toimub parse() funktsioonis. Massiivi pikkus on võetud varuga */
    struct BF_instruction_st **inst_arr = malloc(sizeof(struct BF_instruction_st *) * program_len);

    /* Parsime sisendprogrammi, mille tulemus salvestatakse inst_arr massiivi. */
    parse(inst_arr, program);

    /* Käivitame programmi. */
    run(inst_arr, program_len);

    int i = 0;
    while (1) {
        if (i < 0 || i >= program_len) break;
        if (inst_arr[i] != NULL) {
            free(inst_arr[i]);
            i++;
        } else {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }

    free(inst_arr);
}

void interpret3(char *program) {
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
    while (1) {
        if (i < 0 || i >= program_len) break;
        if (inst_arr[i] != NULL) {
            free(inst_arr[i]);
            i++;
        } else {
            /* Suurendame indeksit iseseisvalt. */
            i++;
        }
    }

    free(inst_arr);
}

int main(int argc, char **argv) {
    /* Kontrollime, et programmile anti täpselt üks parameeter (lisaks programmi nimele endale). */
    if (argc != 2) {
        printf("Programmil peab olema üks parameeter, milleks on BF programm!\n");

        /* Tagastame veakoodi. */
        return 1;
    }

    struct stack_st stack = { .len = 0, .size = 0, .arr = NULL};

    /* Käivitame programmi, mille kasutaja andis käsurealt. */
    // interpret(&stack, argv[1]);
    interpret3(argv[1]);

    return 0;
}