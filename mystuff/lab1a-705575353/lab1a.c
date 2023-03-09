/**
NAME: Nevin Liang
EMAIL: nliang868@g.ucla.edu
ID: 705575353
**/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <getopt.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

struct termios new, orig;

void handler(int signal) {
    fprintf(stderr, "SIGPIPE has been raised\n");
    return;
}

void reg() {
    while (1) {
        char buf[256];
        int inp;
        if ((inp = read(STDIN_FILENO, buf, 256)) < 0) {
            fprintf(stderr, "error reading from stdin: %s\n", strerror(errno));
            exit(1);
        }
        for (char* e = buf; e < buf + inp; e++) {
            if (*e == 0x04) {
                if (write(STDOUT_FILENO, "^D", 2) < 0) {
                    fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                    exit(1);
                }
                if (tcsetattr(STDIN_FILENO, TCSANOW, &orig) < 0) {
                    fprintf(stderr, "error setting attribute for stdin: %s\n", strerror(errno));
                    exit(1);
                }
                exit(0);
            }
            else if (*e == 0x0D || *e == 0x0A) {
                if (write(STDOUT_FILENO, "\r\n", 2) < 0) {
                    fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                    exit(1);
                }
            }
            else {
                if (write(STDOUT_FILENO, e, 1) < 0) {
                    fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                    exit(1);
                }
            }
        }
    }
}

void shell() {
    signal(SIGPIPE, handler);
    int pipePC[2], pipeCP[2];
    if (pipe(pipePC) < 0) {
        fprintf(stderr, "error in pipe parent->child creation: %s\n", strerror(errno));
        exit(1);
    }
    if (pipe(pipeCP) < 0) {
        fprintf(stderr, "error in pipe child->parent creation: %s\n", strerror(errno));
        exit(1);
    }
    int pid;
    if ((pid = fork()) < 0) {
        fprintf(stderr, "error forking: %s\n", strerror(errno));
        exit(1);
    }
    if (pid == 0) {
        if (close(pipePC[1]) < 0) {
            fprintf(stderr, "error closing PC pipe write fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(pipeCP[0]) < 0) {
            fprintf(stderr, "error closing CP pipe read fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(STDIN_FILENO) < 0) {
            fprintf(stderr, "error closing stdin fd: %s\n", strerror(errno));
            exit(1);
        }
        if (dup(pipePC[0]) < 0) {
            fprintf(stderr, "error duping PC pipe read fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(pipePC[0]) < 0) {
            fprintf(stderr, "error closing PC pipe read fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(STDOUT_FILENO) < 0) {
            fprintf(stderr, "error closing stdout fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(STDERR_FILENO) < 0) {
            fprintf(stderr, "error closing stderr fd: %s\n", strerror(errno));
            exit(1);
        }
        if (dup(pipeCP[1]) < 0) {
            fprintf(stderr, "error duping CP pipe write fd: %s\n", strerror(errno));
            exit(1);
        }
        if (dup(pipeCP[1]) < 0) {
            fprintf(stderr, "error duping CP pipe write fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(pipeCP[1]) < 0) {
            fprintf(stderr, "error closing CP pipe write fd: %s\n", strerror(errno));
            exit(1);
        }

        execlp("/bin/bash", "/bin/bash", (char*)NULL);
        fprintf(stderr, "error exec bash process: %s\n", strerror(errno));
        exit(1);
    }

    else {
        if (close(pipePC[0]) < 0) {
            fprintf(stderr, "error closing PC pipe read fd: %s\n", strerror(errno));
            exit(1);
        }
        if (close(pipeCP[1]) < 0) {
            fprintf(stderr, "error closing CP pipe write fd: %s\n", strerror(errno));
            exit(1);
        }

        struct pollfd term[2];
        int fromBash = 0, fromKeyb = 1;

        term[fromBash].fd = pipeCP[0];
        term[fromKeyb].fd = STDIN_FILENO;

        term[fromBash].events = term[fromKeyb].events = POLLIN | POLLHUP | POLLERR;

        int polling = 1;
        while (polling) {
            int retval = poll(term, (unsigned long)2, -1);
            if (retval < 0) {
                fprintf(stderr,"error while polling: %s\n", strerror(errno));
                exit(1);
            }
            for (int i = 0; i < 2; i++) {
                if (term[i].revents != 0) {
                    if (term[i].revents & POLLIN) {
                        char buf[256];
                        if (i == fromBash) {
                            int inp;
                            if ((inp = read(pipeCP[0], buf, 256)) < 0) {
                                fprintf(stderr, "error reading from CP pipe: %s\n", strerror(errno));
                                exit(1);
                            }
                            for (char* e = buf; e < buf + inp; e++) {
                                if (*e == 0x04) {
                                    polling = 0;
                                }
                                else if (*e == 0x0A) {
                                    if (write(STDOUT_FILENO, "\r\n", 2) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                }
                                else {
                                    if (write(STDOUT_FILENO, e, 1) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                }  
                            }
                        }
                        if (i == fromKeyb) {
                            int inp = read(STDIN_FILENO, buf, 256);
                            for (char* e = buf; e < buf + inp; e++) {
                                if (*e == 0x0A || *e == 0x0D) {
                                    if (write(STDOUT_FILENO, "\r\n", 2) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                    if (write(pipePC[1], "\n", 1) < 0) {
                                        fprintf(stderr, "error writing to pipePC: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                }
                                else if (*e == 0x04) {
                                    if (write(STDOUT_FILENO, "^D", 2) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                    if (pipePC[1] != -1) {
                                        if (close(pipePC[1]) < 0) {
                                            fprintf(stderr, "error closing PC pipe write fd: %s\n", strerror(errno));
                                            exit(1);
                                        }
                                        pipePC[1] = -1;
                                    }
                                }
                                else if (*e == 0x03) {
                                    if (write(STDOUT_FILENO, "^C", 2) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                    if (kill(pid, SIGINT) < 0) {
                                        fprintf(stderr, "error sending kill signal: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                }
                                else {
                                    if (write(STDOUT_FILENO, e, 1) < 0) {
                                        fprintf(stderr, "error writing to stdout: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                    if (write(pipePC[1], e, 1) < 0) {
                                        fprintf(stderr, "error writing to pipePC: %s\n", strerror(errno));
                                        exit(1);
                                    }
                                }
                            }
                        }
                    }
                    else if (term[i].revents & (POLLHUP | POLLERR))
                        polling = 0;
                }
            }
        }
        if (pipePC[1] != -1) {
            if (close(pipePC[1]) < 0) {
                fprintf(stderr, "error closing write end of pipePC: %s\n", strerror(errno));
                exit(1);
            }
        }
        int retstat;
        if (waitpid(pid, &retstat, 0) < 0) {
            fprintf(stderr, "error child has not exited: %s\n", strerror(errno));
            exit(1);
        }
        int exitsig = retstat & 0x007f, status = (retstat & 0xff00) >> 8;
        fprintf(stderr, "SHELL EXIT SIGNAL=%d STATUS=%d\r\n", exitsig, status);
        if (tcsetattr(STDIN_FILENO, TCSANOW, &orig) < 0) {
            fprintf(stderr, "error setting attribute for stdin: %s\n", strerror(errno));
            exit(1);
        }
        exit(0);
    }
}

int isShell(int argc, char **argv) {
    static struct option opts[] = {
        {"shell", no_argument, 0, 's'},
        {0, 0, 0, 0}
    };
    int opt_it = 0;
    while (1) {
        int opt = getopt_long(argc, argv, "s", opts, &opt_it);
        if (opt == -1) break;
        if (opt == 's') return 1;
        else {
            fprintf(stderr, "usage: ./lab1a [--shell]\n");
            exit(1);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int sf = isShell(argc, argv);

    if (tcgetattr(STDIN_FILENO, &orig) < 0) {
        fprintf(stderr, "error getting attribute for stdin: %s\n", strerror(errno));
        exit(1);
    }
    new = orig;
    new.c_iflag = ISTRIP, new.c_oflag = new.c_lflag = 0;
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new) < 0) {
        fprintf(stderr, "error setting attribute for stdin: %s\n", strerror(errno));
        exit(1);
    }

    (sf) ? shell() : reg();
}