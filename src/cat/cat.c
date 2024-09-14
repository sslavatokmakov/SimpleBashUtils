#include "cat.h"

int main(int argc, char *argv[]) {
    arguments arg = arguments_parser(argc, argv);
    output(argc, argv, &arg);
    return 0;
}

void output(int argc, char **argv, arguments *arg) {
    for (int i = optind; i < argc; i++) {
        FILE *f = fopen(argv[i], "r");
        if (f != NULL)
            process_file(f, arg);
        else
            printf("no such file");
    }
}

arguments arguments_parser(int argc, char *argv[]) {
    int opt;
    arguments arg = {0};
    struct option long_options[] = {{"number",          0, 0, 'n'},
                                    {"number-nonblank", 0, 0, 'b'},
                                    {"squeeze-blank",   0, 0, 's'},
                                    {0,                 0, 0, 0}};

    while ((opt = getopt_long(argc, argv, "benstvTE", long_options, 0)) != -1) {
        switch (opt) {
            case 'b':
                arg.b = true;
                break;
            case 'e':
                arg.e = true;
                arg.v = true;
                break;
            case 'n':
                arg.n = true;
                break;
            case 's':
                arg.s = true;
                break;
            case 't':
                arg.t = true;
                arg.v = true;
                break;
            case 'v':
                arg.v = true;
                break;
            case 'T':
                arg.t = true;
                break;
            case 'E':
                arg.e = true;
                break;
            default:
                printf("cat: illegal option -- %s\nusage: cat [-benstvTE] [file ...]",
                       argv[optind]);
                break;
        }
    }
    if (arg.n && arg.b) {
        arg.n = false;
    }
    return arg;
}

void process_file(FILE *f, arguments *option) {
    int line_counter = 0;
    int non_empty_line_counter = 0;
    bool flag_b = false, flag_n = false, flag_e = false;
    int flag_s = 0;
    int ch;
    while ((ch = fgetc(f)) != EOF) {
        if (option->s == 1) {
            if (check_flag_s(ch, &flag_s)) {
                continue;
            }
        }
        if (option->b == 1) {
            if (check_flag_b(ch, &flag_b)) {
                non_empty_line_counter++;
                printf("%6d\t", non_empty_line_counter);
            }
        }
        if (option->n == 1) {
            if (check_flag_n(ch, &flag_n)) {
                line_counter++;
                printf("%6d\t", line_counter);
            }
        }
        if (option->e == 1) {
            if (ch == '\n' && flag_e == 0 && option->b == 1) printf("      \t");

            if (check_flag_e(ch, &flag_e)) {
                putchar('$');
            }
        }
        if (option->t == 1) {
            if (ch == '\t') {
                printf("^I");
                continue;
            }
        }
        if (option->v == 1) {
            if (check_flag_v(ch)) {
                printf("^%c", (ch + 1) % 128 + 63);
                continue;
            }
        }
        putchar(ch);
    }
    fclose(f);
}

bool check_flag_s(int ch, int *flag_s) {
    bool flag = false;
    if (ch == '\n') {
        (*flag_s)++;
        if (*flag_s >= 2) {
            flag = true;
        }
    } else {
        *flag_s = -1;
    }
    return flag;
}

bool check_flag_b(int ch, bool *flag_b) {
    bool flag = false;
    if (ch != '\n') {
        if (*flag_b == false) {
            flag = true;
            *flag_b = true;
        }
    } else {
        *flag_b = false;
    }
    return flag;
}

bool check_flag_n(int ch, bool *flag_n) {
    bool flag = false;
    if (*flag_n == false) {
        *flag_n = true;
        flag = true;
    }
    if (ch == '\n') {
        *flag_n = false;
    }
    return flag;
}

bool check_flag_e(int ch, bool *flag_e) {
    bool flag = false;
    if (ch == '\n') {
        *flag_e = false;
        flag = true;
    } else {
        *flag_e = true;
    }
    return flag;
}

bool check_flag_v(int ch) {
    bool flag = false;
    if ((ch <= 31 || ch == 127) && (ch != '\n' && ch != '\t')) {
        flag = true;
    }
    return flag;
}