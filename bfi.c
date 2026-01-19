#include <stdio.h>
#include "mem.h"
#include "stack.h"

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

int main(int argc, char **argv) {
    /* Kontrollime, et programmile anti täpselt üks parameeter (lisaks programmi nimele endale). */
    if (argc != 2) {
        printf("Programmil peab olema üks parameeter, milleks on BF programm!\n");

        /* Tagastame veakoodi. */
        return 1;
    }

    struct stack_st stack = { .len = 0, .size = 0, .arr = NULL};

    /* Käivitame programmi, mille kasutaja andis käsurealt. */
    interpret(&stack, argv[1]);

    return 0;
}