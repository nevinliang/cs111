// NAME: Nevin Liang
// EMAIL: nliang868@g.ucla.edu
// ID: 705575353

#include "SortedList.h"

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>

SortedListElement_t* allnodes;
SortedList_t sorted = {.prev = &sorted, .next = &sorted, .key = NULL};
int threads = 1, iters = 1, opt_yield, opt_sync;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long spin = 0;

void sighandler(int error) {
    fprintf(stderr, "SEGFAULT WITH ERROR %d\n", error);
    exit(2);
}

void option_parse(int argc, char **argv) {
    static struct option opts[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", required_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int opt_it = 0;
    while (1) {
        int opt = getopt_long(argc, argv, "t:i:", opts, &opt_it);
        if (opt == -1) break;
        switch (opt) {
            case 't':
                threads = atoi(optarg);
                break;
            case 'i':
                iters = atoi(optarg);
                break;
            case 'y':
                for (unsigned int i = 0; i < strlen(optarg); i++) {
                    if (optarg[i] == 'i') opt_yield |= INSERT_YIELD;
                    if (optarg[i] == 'd') opt_yield |= DELETE_YIELD;
                    if (optarg[i] == 'l') opt_yield |= LOOKUP_YIELD;
                    if (optarg[i] != 'i' && optarg[i] != 'd' && optarg[i] != 'l') {
                        fprintf(stderr, "usage: ./lab2_list [--threads=#] [--iterations=#] \
                                [--yield=idl] [--sync=sm]\n");
                        exit(1);
                    }
                }
                break;
            case 's':
                if (optarg[0] == 's') opt_sync = 1;
                else if (optarg[0] == 'm') opt_sync = 2;
                else {
                    fprintf(stderr, "usage: ./lab2_list [--threads=#] [--iterations=#] \
                                [--yield=idl] [--sync=sm]\n");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "usage: ./lab2_list [--threads=#] [--iterations=#] \
                                [--yield=idl] [--sync=sm]\n");
                exit(1);
        }
    }
}

void init_allnodes(int threads, int iters) {
    allnodes = malloc(threads * iters * sizeof(SortedListElement_t));
    int length = 8;
    const char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    char randstr[threads * iters][length];
    for (int i = 0; i < threads * iters; i++) {
        for (int j = 0; j < length - 1; j++)
            randstr[i][j] = alpha[rand() % 26];
        randstr[i][length - 1] = '\0';
        allnodes[i].key = randstr[i];
        allnodes[i].next = allnodes[i].prev = NULL;
    }
}

void lock() {
    if (opt_sync == 1)
        while (__sync_lock_test_and_set (&spin, 1));
    if (opt_sync == 2)
        if (pthread_mutex_lock(&mutex)) {
            fprintf(stderr, "mutex lock error\n");
        }
}

void unlock() {
    if (opt_sync == 1)
        __sync_lock_release(&spin);
    if (opt_sync == 2)
        if (pthread_mutex_unlock(&mutex)) {
            fprintf(stderr, "mutex unlock error\n");
        }
}

void* thread_list(void* arg) {
    lock();
    int* index = arg;
    int thread = *index;
    unlock();

    for (int iter = 0; iter < iters; iter++) {
        lock();
        SortedList_insert(&sorted, &allnodes[thread * iters + iter]);
        unlock();
    }
    lock();
    if (SortedList_length(&sorted) == -1) {
        fprintf(stderr, "length error\n");
        exit(2);
    }
    unlock();
    for (int iter = 0; iter < iters; iter++) {
        lock();
        SortedListElement_t* node = SortedList_lookup(&sorted, allnodes[thread * iters + iter].key);
        if (node == NULL) {
            fprintf(stderr, "list corrupted\n");
            exit(2);
        }
        SortedList_delete(&allnodes[thread * iters + iter]);
        unlock();
    }
    return 0;
}

int main(int argc, char **argv) {
    signal(SIGSEGV, sighandler);

    option_parse(argc, argv);
    init_allnodes(threads, iters);

    pthread_t* allthreads = malloc(threads * sizeof(pthread_t));

    int tindex[threads];
    for (int i = 0; i < threads; i++)
        tindex[i] = i;

    struct timespec tpbegin, tpend;
    clock_gettime(CLOCK_REALTIME, &tpbegin);
    for (int i = 0; i < threads; i++)
        pthread_create(&allthreads[i], NULL, thread_list, &tindex[i]);
    for (int i = 0; i < threads; i++)
        pthread_join(allthreads[i], NULL);
    clock_gettime(CLOCK_REALTIME, &tpend);
    free(allnodes);
    free(allthreads);

    if (SortedList_length(&sorted) != 0) {
        fprintf(stderr, "Final Length Not 0\n");
        exit(2);
    }

    long ns = tpend.tv_nsec - tpbegin.tv_nsec, sec = tpend.tv_sec - tpbegin.tv_sec;
    long tt = sec * 1000000000L + ns, ops = threads * iters * 3L;
    
    char type[16] = "list-";

    if (opt_yield == 0) strcat(type, "none");
    if (opt_yield & INSERT_YIELD) strcat(type, "i");
    if (opt_yield & DELETE_YIELD) strcat(type, "d");
    if (opt_yield & LOOKUP_YIELD) strcat(type, "l");
    strcat(type, "-");

    if (opt_sync == 0) strcat(type, "none");
    else if (opt_sync == 1) strcat(type, "s");
    else if (opt_sync == 2) strcat(type, "m");

    printf("%s,%d,%d,%d,%ld,%ld,%ld\n", type, threads, iters, 1, ops, tt, tt / ops);
}