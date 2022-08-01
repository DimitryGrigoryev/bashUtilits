#include "s21_grep.h"

int main(int argc, char** argv) {
    Template *template = (Template *)malloc(sizeof(Template) * (argc - 2));
    int def_tmplt_n_argc = 0;
    t_flags opt = s21_initial_flags();
    int num_template = 0;
    int num_file = 0;

    int ERROR = 0;
    if (argc > 0) {
        int count = 1;
        while (argv[count] && !ERROR) {  // parser flag
            size_t n = strlen(argv[count]);
            if (argv[count][0] == '-') {
                size_t i;
                for (i = 1; i < n; i++) {
                    if (argv[count][i] == ' ') {
                        ERROR = 0;  // stop char
                    } else if (argv[count][i] == 'e') {
                        opt.e = 1;
                        template[num_template].template = argv[count + 1];
                        num_template += 1;
                        break;
                    } else if (argv[count][i] == 'i') {
                        opt.i = 1;
                    } else if (argv[count][i] == 'v') {
                        opt.v = 1;
                    } else if (argv[count][i] == 'c') {
                        opt.c = 1;
                    } else if (argv[count][i] == 'l') {
                        opt.l = 1;
                    } else if (argv[count][i] == 'n') {
                        opt.n = 1;
                    } else if (argv[count][i] == 'h') {
                        opt.h = 1;
                    } else if (argv[count][i] == 's') {
                        opt.s = 1;
                    } else {
                        ERROR = 1;
                    }
                }
                if (!ERROR) {
                    if (strcmp(argv[count], "-e") != 0) memset(argv[count], '\0', strlen(argv[count]));
                } else  {
                    if (!opt.s) printf("grep: invalid option -- '%c'\n", argv[count][i]);
                }
            }
            count++;
        }
        if (!ERROR && argc > 1 && !opt.e) {
            for (int i = 1; i < argc; i++) {
                while (!num_template && (argv[i][0] != '\0')) {
                    template[0].template = argv[i];
                    def_tmplt_n_argc = i;
                    num_template += 1;
                }
                if (num_template) break;
            }
        }
        for (int i = 1; i < argc; i++) {
            if (i != def_tmplt_n_argc) {
                while (argv[i][0] != '\0') {
                    if (strcmp(argv[i], "-e") == 0) {
                        i = i + 2;
                        break;
                    } else {
                        num_file +=1;
                        break;
                    }
                }
            }
        }
    }
    if (!ERROR) {
        int REG_FL = REG_EXTENDED;
        size_t len = 0;
        int compare = 0;
        int count_file = 1;
        if (opt.v) compare = REG_NOMATCH;
        if (opt.i) REG_FL = REG_ICASE;
        for (int count = 1; count < argc; count++) {
            if (count != def_tmplt_n_argc) {
                while (!ERROR && argv[count][0] != '\0') {
                    if (strcmp(argv[count], "-e") == 0) {
                        count = count + 1;
                        break;
                    } else {
                        char *temp_line = NULL;
                        char *pt = argv[count];
                        FILE *fp = fopen(pt, "rb");
                        if (!fp) {
                            ERROR = 2;
                            if (!opt.s) printf("grep: %s: No such file or directory\n", argv[count]);
                        } else {
                            int rep = 0;
                            ssize_t find_success = 0;
                            int over = 0;
                            int num_str = 0;
                            int val = -1;
                            while (rep != EOF && !over) {
                                regex_t regex;
                                rep = getline(&temp_line, &len, fp);
                                num_str += 1;
                                if (temp_line && (rep != EOF)) {
                                    int success = 1;
                                    int count_success = 1;
                                    for (int i = 0; i < num_template; i++) {
                                        val = regcomp(&regex, template[i].template, REG_FL);
                                        if (val == 0) {
                                            count_success = regexec(&regex, temp_line, 0, NULL, 0);
                                        } else {
                                            if (!opt.s) printf("Regex compilation error\n");
                                        }
                                        if (!count_success) {
                                            if (count_success == compare && opt.l) {
                                                over = 1;
                                                if (count_file > 1) printf("\n");
                                                printf("%s\n", pt);
                                                count_file++;
                                                break;
                                            }
                                            success = count_success;
                                        } else {
                                            if (count_success == compare && opt.l) {
                                                over = 1;
                                                if (count_file > 1) printf("\n");
                                                printf("%s\n", pt);
                                                count_file++;
                                                break;
                                            }
                                        }
                                    }
                                    if (success == compare) find_success +=1;
                                    if (success == compare && !opt.c && !over) {
                                        if (num_file > 1 && !opt.h) printf("%s:", argv[count]);
                                        if (opt.n) printf("%d:", num_str);
                                        if (temp_line) printf("%s", temp_line);
                                    }
                                }
                                if (val == 0) regfree(&regex);
                            }
                            if (opt.c && !over) {
                                if (count_file > 1) printf("\n");
                                if (num_file > 1 && !opt.h) printf("%s:", argv[count]);
                                printf("%zd\n", find_success);
                                count_file++;
                            }
                        }
                        if (temp_line) free(temp_line);
                        if (fp) fclose(fp);
                        break;
                    }
                }
            }
        }
    }
    if (num_template > 0) {
        for (int i = 1; i < num_template; i++) {
            if (template[i].template) free(template[i].template);
        }
    }
    if (template) free(template);
    return 0;
}

t_flags s21_initial_flags(void) {
     t_flags opt;
     opt.e = 0;
     opt.i = 0;
     opt.v = 0;
     opt.c = 0;
     opt.l = 0;
     opt.n = 0;
     opt.h = 0;
     opt.s = 0;
     opt.n = 0;
     return (opt);
}
