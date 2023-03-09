// NAME: Nevin Liang
// EMAIL: nliang868@g.ucla.edu
// ID: 705575353

#include "SortedList.h"
#include <string.h>
#include <pthread.h>

// the following funcs only work if head is initialized as prev and next to itself.
void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
    SortedListElement_t* iter = list->next;
    while (iter->key != NULL && strcmp(element->key, iter->key) > 0)
        iter = iter->next;
    if(opt_yield & INSERT_YIELD) sched_yield();
    iter->prev->next = element;
    element->prev = iter->prev;
    iter->prev = element;
    element->next = iter;
}

int SortedList_delete(SortedListElement_t *element) {
    if(element->prev->next != element || element->next->prev != element) return 1;
    element->next->prev = element->prev;
    if(opt_yield & DELETE_YIELD) sched_yield();
    element->prev->next = element->next;
    return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
    SortedListElement_t* curr = list->next;
    while (curr->key != NULL) {
        if (strcmp(curr->key, key) == 0) break;
        curr = curr->next;
    }
    if(opt_yield & LOOKUP_YIELD) sched_yield();
    if (curr->key == NULL) return NULL;
    else return curr;
}

int SortedList_length(SortedList_t *list) {
    int len = 0;
    SortedListElement_t* curr = list->next;
    while (curr->key != NULL) {
        if (curr->prev->next != curr || curr->next->prev != curr) return -1;
        len++;
        curr = curr->next;
    }
    if(opt_yield & LOOKUP_YIELD) sched_yield();
    return len;
}