#include "queue.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

void queue_init(queue* q){
    (*q) = NULL;
}

void queue_push(queue* q, tree t){
    queue nn = (queue)(malloc(sizeof(qNode)));
    nn->data = t;
    nn->next = NULL;
    queue temp = (*q);
    if ((*q) == NULL){
        (*q) = nn;
        return;
    }
    while (temp->next != NULL){
        temp = temp->next;
    }
    temp->next = nn;
    return;
}

bool queue_empty(queue q){
    if (q == NULL){
        return true;
    }
    return false;
}

int queue_size(queue q){
    int count = 0;
    while (q != NULL){
        count++;
        q = q->next;
    }
    return count;
}

void queue_pop(queue* q){
    if ((*q) == NULL){
        return;
    }
    queue temp = (*q);
    (*q) = (*q)->next;
    free(temp);
}

tree queue_front(queue q){
    return q->data;
}
