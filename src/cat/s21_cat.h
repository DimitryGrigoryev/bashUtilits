#ifndef  SRC_S21_CAT_H_
#define  SRC_S21_CAT_H_


#define  _GNU_SOURCE
#include <stdio.h>
#include <string.h>

typedef struct flags {
    int b;
    int e;
    int E;
    int s;
    int t;
    int T;
    int n;
} t_flags;

t_flags s21_initial_flags(void);

void show_noprint(int print_c);

#endif  // SRC_S21_CAT_H_