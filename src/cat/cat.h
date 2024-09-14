#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

typedef struct arguments {
    bool v;
    bool b;
    bool e;
    bool n;
    bool s;
    bool t;
} arguments;

void output(int argc, char **argv, arguments *arg);

void process_file(FILE *f, arguments *option);

arguments arguments_parser(int argc, char *argv[]);

bool check_flag_s(int ch, int *flag_s);

bool check_flag_b(int ch, bool *flag_b);

bool check_flag_n(int ch, bool *flag_n);

bool check_flag_e(int ch, bool *flag_e);

bool check_flag_v(int ch);