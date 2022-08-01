#ifndef SRC_S21_GREP_H_
#define SRC_S21_GREP_H_

#define  _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <regex.h>

typedef struct flags {
    int e;
    int i;
    int v;
    int c;
    int l;
    int n;
    int h;
    int s;
} t_flags;

typedef struct Template {
    char *template;
} Template;

t_flags s21_initial_flags(void);

#endif  // SRC_S21_GREP_H_