/**
NAME: Nevin Liang
EMAIL: nliang868@g.ucla.edu
ID: 705575353
**/

#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>

typedef long long ll;

typedef struct addargs {
    ll *pointer;
    int iters;
    int sync;
} addargs_t;

int opt_yield;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
long spin = 0;

void add(ll *pointer, ll value) {
    ll sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
}

void madd(ll *pointer, ll value) {
    if (pthread_mutex_lock(&mutex)) {
        fprintf(stderr, "mutex lock error\n");
        exit(2);
    }
    ll sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    if (pthread_mutex_unlock(&mutex)) {
        fprintf(stderr, "mutex unlock error\n");
        exit(2);
    }
}

void sadd(ll *pointer, ll value) {
    while (__sync_lock_test_and_set (&spin, 1)); 
    ll sum = *pointer + value;
    if (opt_yield) sched_yield();
    *pointer = sum;
    __sync_lock_release(&spin);
}

void cadd(ll *pointer, ll value) {
    ll prev, sum;
    do {
        sum = (prev = *pointer) + value;
        if (opt_yield) sched_yield();
    } while (__sync_val_compare_and_swap (pointer, prev, sum) != prev);
}

void* thread_add(void* arg) {
    addargs_t* addarg = arg;
    int val[2] = {1, -1};
    void (*func_ptr[])(ll *pointer, ll value) = {add, madd, sadd, cadd};
    for (int v = 0; v < 2; v++)
        for (int i = 0; i < addarg->iters; i++)
            (*func_ptr[addarg->sync])(addarg->pointer, val[v]);
    return 0;
}

int main(int argc, char **argv) {

    static struct option opts[] = {
        {"threads", required_argument, 0, 't'},
        {"iterations", required_argument, 0, 'i'},
        {"yield", no_argument, 0, 'y'},
        {"sync", required_argument, 0, 's'},
        {0, 0, 0, 0}
    };

    int opt_it = 0;
    unsigned int threads = 1, iters = 1;
    int sync = 0;
    char sync_flag;

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
                opt_yield = 1;
                break;
            case 's':
                sync_flag = optarg[0];
                if (sync_flag == 'm') sync = 1;
                else if (sync_flag == 's') sync = 2;
                else if (sync_flag == 'c') sync = 3;
                else {
                    fprintf(stderr, "usage: ./lab2_add [--threads=#] [--iterations=#] [--yield] [--sync=msc]\n");
                    exit(1);
                }
                break;
            default:
                fprintf(stderr, "usage: ./lab2_add [--threads=#] [--iterations=#] [--yield] [--sync=msc]\n");
                exit(1);
        }
    }

    ll counter = 0;

    addargs_t thread_arg = {.pointer = &counter, .iters = iters, .sync = sync};
    pthread_t* threadlist = malloc(threads * sizeof(pthread_t));

    struct timespec tpbegin, tpend;
    clock_gettime(CLOCK_REALTIME, &tpbegin);
    
    for (unsigned int i = 0; i < threads; i++)
        pthread_create(&threadlist[i], NULL, thread_add, &thread_arg);
    for (unsigned int i = 0; i < threads; i++)
        pthread_join(threadlist[i], NULL);
    
    clock_gettime(CLOCK_REALTIME, &tpend);
    free(threadlist);

    long ns = tpend.tv_nsec - tpbegin.tv_nsec;
    long sec = tpend.tv_sec - tpbegin.tv_sec;

    long tt = sec * 1000000000L + ns;
    int ops = threads * iters * 2;

    char type[16] = "add-";
    if (opt_yield)
        strcat(type, "yield-");

    if (sync > 0) {
        char mode[2] = {sync_flag, '\0'};
        strcat(type, mode);
    }
    else
        strcat(type, "none\0");
    
    printf("%s,%d,%d,%d,%ld,%ld,%lld\n", type, threads, iters, ops, tt, tt / ops, counter);
}