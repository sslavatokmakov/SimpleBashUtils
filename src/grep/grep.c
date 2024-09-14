#include "grep.h"

void pattern_add(arguments *arg, char *pattern) {
    if (arg->len_pattern != 0) {
        strcat(arg->pattern + arg->len_pattern, "|");
        arg->len_pattern++;
    }
    arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(arguments *arg, char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (f == NULL) {
        if (!arg->s) perror(filepath);
    }
    char *line = NULL;
    size_t memlen = 0;
    int read;
    while ((read = getline(&line, &memlen, f)) != -1) {
        if (line[read - 1] == '\n') {
            line[read - 1] = '\0';
        }
        pattern_add(arg, line);
    }
    free(line);
    fclose(f);
}

void print_match(regex_t *reg, char *line, int v) {
    regmatch_t match;
    int offset = 0;

    for (size_t i = 0; i < strlen(line); i++) {
        int result = regexec(reg, line + offset, 1, &match, 0);
        if (result == 0 && !v) {
            for (int j = match.rm_so; j < match.rm_eo; i++, j++) {
                putchar(line[j]);
            }
            offset += match.rm_eo;
            putchar('\n');
        }
        if (result == 1 && v) putchar(line[i]);
    }
    if (v && line[strlen(line) - 1] != '\n') putchar('\n');
}

arguments arguments_parser(int argc, char *argv[]) {
    arguments arg = {0};
    int opt;
    while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
        switch (opt) {
            case 'e':
                arg.e = true;
                pattern_add(&arg, optarg);
                break;
            case 'i':
                arg.i = true;
                break;
            case 'v':
                arg.v = true;
                break;
            case 'c':
                arg.c = true;
                break;
            case 'l':
                arg.l = true;
                break;
            case 'n':
                arg.n = true;
                break;
            case 'h':
                arg.h = true;
                break;
            case 's':
                arg.s = true;
                break;
            case 'f':
                arg.f = true;
                add_reg_from_file(&arg, optarg);
                break;
            case 'o':
                arg.o = true;
                break;
        }
    }
    if (arg.len_pattern == 0) {
        pattern_add(&arg, argv[optind++]);
    }
    if (argc - optind == 1) {
        arg.h = true;
    }
    return arg;
}

void output_line(char *line, int n) {
    for (int i = 0; i < n; i++) {
        putchar(line[i]);
    }
    if (line[n - 1] != '\n') {
        putchar('\n');
    }
}

void process_file(arguments arg, char *path, regex_t *reg) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        if (!arg.s) perror(path);
    }
    char *line = NULL;
    size_t memlen;
    int read;
    int line_count = 1;
    int c = 0;

    while ((read = getline(&line, &memlen, f)) != -1) {
        int result = regexec(reg, line, 0, NULL, 0);
        if ((result == 0 && !arg.v) || (result == 1 && arg.v)) {
            if (!arg.c && !arg.l) {
                if (!arg.h) printf("%s:", path);
                if (arg.n) printf("%d:", line_count);
                if (arg.o) {
                    print_match(reg, line, arg.v);
                } else
                    output_line(line, read);
            }
            c++;
        }
        line_count++;
    }
    if (arg.c) {
        if (!arg.h) printf("%s:", path);
        if (arg.l)
            printf("1\n");
        else
            printf("%d\n", c);
    }
    if (arg.l && c > 0) {
        printf("%s\n", path);
    }
    free(line);
    fclose(f);
}

void output(arguments arg, int argc, char **argv) {
    regex_t reg;
    int error =
            regcomp(&reg, arg.pattern, (arg.i ? REG_ICASE : 0) | REG_EXTENDED);
    if (error) perror("error");
    for (int i = optind; i < argc; i++) {
        process_file(arg, argv[i], &reg);
    }
    regfree(&reg);
}

int main(int argc, char **argv) {
    arguments arg = arguments_parser(argc, argv);
    arg.file_count = argc - optind;
    output(arg, argc, argv);
    return 0;
}