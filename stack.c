#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void stack_push(struct stack_st *s, int element) {
    if (s->size == 0 || s->arr == NULL){
        s->size = 1;

        s->arr = malloc(sizeof(int) * s->size);

        if (s->arr == NULL) {
            printf("Mälu otsas!\n");
            return;
        }

        s->len = 0;
    } else if (s->len == s->size) {
        int tmp_size = s->size * 2;
        int *tmp_arr = NULL;

        tmp_arr = realloc(s->arr, sizeof(int) * tmp_size);
        
        if (tmp_arr == NULL) {
            printf("Mäluala suuruse muutmine ebaõnnestus.\n");
            return;
        }

        s->arr = tmp_arr;
        s->size = tmp_size;
    }

    s->arr[s->len] = element;

    s->len++;

    return;
}

int stack_pop(struct stack_st *s) {
    if (s->size == 0 || s->len == 0 || s->arr == NULL) {
        return 0;
    }

    s->len--;

    if (s->len == 0) {
        int res = s->arr[s->len];
        s->arr = NULL;
        s->size = 0;
        return res;
    }

    if (s->len < s->size / 2) {
        int tmp_size = s->size / 2;
        int *tmp_arr = realloc(s->arr, sizeof(int)*tmp_size);

        if (tmp_arr == NULL) {
            printf("Mäluala suuruse muutmine ebaõnnestus.\n");
            return s->arr[s->len];
        }

        s->arr = tmp_arr;
        s->size = tmp_size;
    }

    return s->arr[s->len];
}

int stack_isEmpty(struct stack_st *s) {
    return s->len == 0;
}

int stack_peek(struct stack_st *s) {
    return s->arr[s->len-1];
}

void stack_print(struct stack_st *s) {
    int stack_size_orig = s->len;
    while (!stack_isEmpty(s)) {
        printf("%d\n", stack_pop(s));
    }
    s->len = stack_size_orig;
}