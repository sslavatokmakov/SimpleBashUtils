#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
    bool e, i, v, c, l, n, h, s, f, o;
    char pattern[1024];
    int len_pattern;
    int file_count;
} arguments;

void pattern_add(arguments *arg, char *pattern);

void add_reg_from_file(arguments *arg, char *filepath);

void print_match(regex_t *reg, char *line, int v);

arguments arguments_parser(int argc, char *argv[]);

void output_line(char *line, int n);

void process_file(arguments arg, char *path, regex_t *reg);

void output(arguments arg, int argc, char **argv);
