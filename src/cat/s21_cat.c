#include "s21_cat.h"

int main(int argc, char** argv) {
    t_flags opt = s21_initial_flags();
    int ERROR = 0;
    if (argc > 0) {
        int count = 1;
        size_t i, n;
        while (argv[count] && !ERROR) {
            if (argv[count][0] == '-') {
                for (i = 1, n = strlen(argv[count]); i < n; i++) {
                    if (argv[count][1] == '\0' || argv[count][i] == 'u') {
                        ERROR = 0;  // stop char
                    } else if (argv[count][i] == 'b') {
                        opt.b = 1;
                        opt.n = 0;
                    } else if (argv[count][i] == 'e') {
                        opt.e = 1;
                    } else if (argv[count][i] == 'E') {
                        opt.E = 1;
                    } else if (argv[count][i] == 's') {
                        opt.s = 1;
                    } else if (argv[count][i] == 'T') {
                        opt.T = 1;
                    } else if (argv[count][i] == 't') {
                        opt.t = 1;
                    } else if (argv[count][i] == 'n') {
                        if (!opt.b) opt.n = 1;
                    } else if (i < 2 && argv[count][1] == '-' && (strcmp(argv[count], "--number"
                                                                                      "-nonblank")) == 0) {
                        opt.b = 1;
                        opt.n = 0;
                        break;
                    } else if (i < 2 && argv[count][1] == '-' && (strcmp(argv[count], "--num"
                                                                                      "ber")) == 0) {
                        if (!opt.b) opt.n = 1;
                        break;
                    } else if (i < 2 && argv[count][1] == '-' && (strcmp(argv[count], "--squeeze"
                                                                                      "-blank")) == 0) {
                        opt.s = 1;
                        break;
                    } else {
                        ERROR = 1;
                    }
                }
                if (!ERROR) memset(argv[count], '\0', strlen(argv[count]));
                else
                    printf("cat: invalid option -- '%c'\n", argv[count][i]);
            }
            count++;
        }
    }

    if (!ERROR) {
        for (int count = 1; count < argc; count++) {
            int lines_b = 1;
            int lines_n = 1;
            int futr_c = '\0';
            while (!ERROR && argv[count][0] != '\0') {
                char *pt = argv[count];
                FILE *file_test;
                file_test = fopen(pt, "rb");

                if (!file_test) {
                    ERROR = 2;
                    printf("cat: %s: No such file or directory or not read\n", argv[count]);
                } else {
                    int print_c = fgetc(file_test);
                    if (print_c != EOF) {
                        int count_n = 0;
                        if (print_c == '\n') count_n = 2;
                        if (opt.n) printf("%6d\t", lines_n++);
                        if (opt.b && print_c != '\n') printf("%6d\t", lines_b++);
                        if ((opt.e || opt.E) && print_c == '\n') {
                                printf("$");
                            }
                        while ((futr_c = fgetc(file_test)) != EOF) {
                            int print = 0;
                            if (print_c == '\n' && opt.s && futr_c == '\n') {
                                count_n += 2;
                                while (futr_c == '\n' && count_n > 2) {
                                    futr_c = fgetc(file_test);
                                    if (futr_c != '\n') {
                                        count_n = 0;
                                    }
                                }
                            } else {
                                count_n = 0;
                            }
                            if (print_c > 126 || print_c < 32) {
                                if (opt.t) {
                                    show_noprint(print_c);
                                    print = 1;
                                } else if (opt.T) {
                                    if (print_c == '\t') {
                                        printf("^I");
                                        print = 1;
                                    } else {
                                        printf("%c", print_c);
                                        print = 1;
                                    }
                                }
                            }
                            if (print_c != EOF && !print)
                            printf("%c", print_c);
                            if (opt.b && print_c == '\n' && futr_c != '\n')
                            printf("%6d\t", lines_b++);
                            if (print_c == '\n' && opt.n)
                            printf("%6d\t", lines_n++);

                            print_c = futr_c;
                            if (opt.E && print_c == '\n') {
                                printf("$");
                            } else if (opt.e) {
                                if (print_c == '\n') {
                                    printf("$");
                                } else if (print_c == '\t') {
                                    print = 1;
                                } else if (print_c > 126 || print_c < 32) {
                                    show_noprint(print_c);
                                }
                            }
                        }
                        if (print_c != EOF) printf("%c", print_c);
                    }
                    memset(argv[count], '\0', strlen(argv[count]));
                    fclose(file_test);
                }
            }
        }
    }
    return 0;
}

t_flags s21_initial_flags(void) {
     t_flags opt;
     opt.b = 0;
     opt.e = 0;
     opt.E = 0;
     opt.s = 0;
     opt.t = 0;
     opt.T = 0;
     opt.n = 0;
     return (opt);
}

void show_noprint(int print_c) {
    if (print_c == 10 || print_c == 13) {
        printf("%c", print_c);
    } else if (print_c == 127) {
        printf("^?");
    } else if (print_c >= 160 && print_c < 255) {
        printf("M-%c", print_c - 128);
    } else if (print_c > 127 && print_c < 160) {
        printf("M-^%c", print_c - 64);
    } else if (print_c >= 255) {
        printf("M-^?");
    } else {
        printf("^%c", print_c + 64);
    }
}

