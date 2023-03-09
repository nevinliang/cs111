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

#define MAX_LISTS 1000

SortedListElement_t* allnodes;
pthread_t* allthreads;
SortedList_t sorted[MAX_LISTS];
int threads = 1, iters = 1, lists = 1, opt_yield, opt_sync, LENGTH = 8;

pthread_mutex_t mutex[MAX_LISTS];
int spin[MAX_LISTS] = {0};

char* randstr;
long locktime = 0;

void exitfunc() {
    free(allnodes), free(allthreads), free(randstr);
}

void sighandler(int error) {
    fprintf(stderr, "SEGFAULT WITH ERROR %d\n", error);
    exit(2);
}

void badflag() {
    fprintf(stderr, "usage: ./lab2_list [--threads=#] [--iterations=#] \
                    [--yield=idl] [--sync=sm] [--lists=#]\n");
    exit(1);
}

void option_parse(int argc, char **argv) {
    static struct option opts[] = {
        {"threads",     required_argument, 0, 't'},
        {"iterations",  required_argument, 0, 'i'},
        {"yield",       required_argument, 0, 'y'},
        {"sync",        required_argument, 0, 's'},
        {"lists",       required_argument, 0, 'l'},
        {0, 0, 0, 0}
    };
    int opt_it = 0;
    while (1) {
        int opt = getopt_long(argc, argv, "t:i:", opts, &opt_it);
        if (opt == -1) break;
        else if (opt == 't') threads = atoi(optarg);
        else if (opt == 'i') iters = atoi(optarg);
        else if (opt == 'l') {
            lists = atoi(optarg);
            if (lists > MAX_LISTS) {
                fprintf(stderr, "TOO MANY LISTS SORRY!\n");
                exit(1);
            }
        }
        else if (opt == 'y')
            for (unsigned int i = 0; i < strlen(optarg); i++)
                if (optarg[i] == 'i')       opt_yield |= INSERT_YIELD;
                else if (optarg[i] == 'd')  opt_yield |= DELETE_YIELD;
                else if (optarg[i] == 'l')  opt_yield |= LOOKUP_YIELD;
                else                        badflag();
        else if (opt == 's')
            if (optarg[0] == 's')       opt_sync = 1;
            else if (optarg[0] == 'm')  opt_sync = 2;
            else                        badflag();
        else badflag();
    }
}

void init_all() {
    allnodes = malloc(threads * iters * sizeof(SortedListElement_t));
    randstr = malloc(threads * iters * LENGTH * sizeof(char));
    for (int i = 0; i < threads * iters; i++) {
        for (int j = 0; j < LENGTH - 1; j++)
            *(randstr + i * LENGTH + j) = 'a' + rand() % 26;
        *(randstr + i * LENGTH + LENGTH - 1) = '\0';
        allnodes[i].key = randstr + i * LENGTH;
        allnodes[i].next = allnodes[i].prev = NULL;
    }
    allthreads = malloc(threads * sizeof(pthread_t));
    for (int i = 0; i < lists; i++) {
        sorted[i].prev = sorted[i].next = &sorted[i];
        sorted[i].key = NULL;
        pthread_mutex_init(&mutex[i], NULL);
    }
}

void lock(int lst) {
    struct timespec ts, te;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    if (opt_sync == 1)
        while (__sync_lock_test_and_set (&spin[lst], 1));
    else if (opt_sync == 2)
        if (pthread_mutex_lock(&mutex[lst]))
            fprintf(stderr, "mutex lock error\n");
    clock_gettime(CLOCK_MONOTONIC, &te);
    locktime += (te.tv_sec - ts.tv_sec) * 1000000000L + (te.tv_nsec - ts.tv_nsec);
}

void unlock(int lst) {
    if (opt_sync == 1)
        __sync_lock_release(&spin[lst]);
    else if (opt_sync == 2)
        if (pthread_mutex_unlock(&mutex[lst]))
            fprintf(stderr, "mutex unlock error\n");
}

int hash(const char* str) {
    int strsum = 0;
    for (int i = 0; i < LENGTH - 1; i++) strsum += (str[i] - 'a');
    return strsum % lists;
}

void* thread_list(void* arg) {
    int* index = arg;
    int thread = *index;

    for (int iter = 0; iter < iters; iter++) {
        int hval = hash(allnodes[thread * iters + iter].key);
        lock(hval);
        SortedList_insert(&sorted[hval], &allnodes[thread * iters + iter]);
        unlock(hval);
    }
    for (int i = 0; i < lists; i++) {
        lock(i);
        if (SortedList_length(&sorted[i]) == -1) {
            fprintf(stderr, "length error\n");
            exit(2);
        }
        unlock(i);
    }
    for (int iter = 0; iter < iters; iter++) {
        int hval = hash(allnodes[thread * iters + iter].key);
        lock(hval);
        SortedListElement_t* node = 
            SortedList_lookup(&sorted[hval], allnodes[thread * iters + iter].key);
        if (node == NULL) {
            fprintf(stderr, "list corrupted\n");
            exit(2);
        }
        SortedList_delete(node);
        unlock(hval);
    }
    return 0;
}

int main(int argc, char **argv) {
    atexit(exitfunc);
    signal(SIGSEGV, sighandler);
    srand(time(0));

    option_parse(argc, argv);
    init_all();
    int tindex[threads];
    for (int i = 0; i < threads; i++) tindex[i] = i;

    struct timespec ts, te;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    for (int i = 0; i < threads; i++)
        pthread_create(&allthreads[i], NULL, thread_list, &tindex[i]);
    for (int i = 0; i < threads; i++)
        pthread_join(allthreads[i], NULL);
    clock_gettime(CLOCK_MONOTONIC, &te);

    for (int i = 0; i < lists; i++)
        if (SortedList_length(&sorted[i]) != 0) {
            fprintf(stderr, "Final Length for a list is not 0\n");
            exit(2);
        }

    long ns = te.tv_nsec - ts.tv_nsec, sec = te.tv_sec - ts.tv_sec;
    long tt = sec * 1000000000L + ns, ops = threads * iters * 3L;
    
    char pref[16] = "list-";
    if (opt_yield == 0)             strcat(pref, "none");
    if (opt_yield & INSERT_YIELD)   strcat(pref, "i");
    if (opt_yield & DELETE_YIELD)   strcat(pref, "d");
    if (opt_yield & LOOKUP_YIELD)   strcat(pref, "l");
    strcat(pref, "-");
    if (opt_sync == 0)              strcat(pref, "none");
    else if (opt_sync == 1)         strcat(pref, "s");
    else if (opt_sync == 2)         strcat(pref, "m");
    
    if (opt_sync == 0) locktime = 0;

    printf("%s,%d,%d,%d,%ld,%ld,%ld,%ld\n", \
            pref, threads, iters, lists, ops, tt, tt / ops, locktime / ops);
}