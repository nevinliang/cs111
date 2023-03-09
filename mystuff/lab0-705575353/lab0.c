/**
NAME: Nevin Liang
EMAIL: nliang868@g.ucla.edu
ID: 705575353
**/

#include <stdio.h>
#include <getopt.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int finput, foutput;

void handler(int signum) {
    fprintf(stderr, "--catch caught/handled seg fault\nsignal number: %d\n", signum);
    exit(4);
}

void actions(int optvals) {
    if (optvals & 1) {
        close(0);
        dup(finput);
        close(finput);
    }
    if (optvals & 2) {
        close(1);
        dup(foutput);
        close(foutput);
    }
    // must register handler before committing the segmentation fault
    if (optvals & 8) {
        signal(SIGSEGV, handler);
    }
    if (optvals & 4) {
        char* pointer = NULL;
        *pointer = '\n';
    }
}

void copyover() {
    while (1) {
        char c;
        int eof = read(0, &c, 1);
        if (eof <= 0) break;
        write(1, &c, 1);
    }
}

int main(int argc, char **argv) {
    int opt, opt_it = 0, optvals = 0;

    static struct option long_options[] = {
        {"input",       required_argument,  0, 'i'},
        {"output",      required_argument,  0, 'o'},
        {"segfault",    no_argument,        0, 's'},
        {"catch",       no_argument,        0, 'c'},
        {0, 0, 0, 0}
    };
    
    while (1) {
        opt = getopt_long(argc, argv, "i:o:sc", long_options, &opt_it);
        if (opt == -1) break;
        switch(opt) {
            case 'i':
                optvals |= 1;
                finput = open(optarg, O_RDONLY);
                if (finput < 0) {
                    fprintf(stderr, "--input caused error\n%s couldn't be opened\n"
                                    "Error: %s\n", optarg, strerror(errno));
                    exit(2);
                }
                break;
            case 'o':
                optvals |= 2;
                foutput = open(optarg, O_WRONLY | O_CREAT, S_IRWXU);
                if (foutput < 0) {
                    fprintf(stderr, "--output caused error\n%s couldn't be opened\n"
                                    "Error: %s\n", optarg, strerror(errno));
                    exit(3);
                }
                break;
            case 's':
                optvals |= 4;
                break;
            case 'c':
                optvals |= 8;
                break;
            default:
                fprintf(stderr, "usage: ./lab0 [--input=filename] [--output=filename]"
                                " [--segfault] [--catch]\n");
                exit(1);
        }
    }
    actions(optvals);
    copyover();
    exit(0);
}