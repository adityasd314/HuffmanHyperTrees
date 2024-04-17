#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "node.h"
#include "linkedlist.h"
#include "tree.h"

void insert_node_ll(ll* list, char* key, int value){
    ll nn = (ll)(malloc(sizeof(node)));
    nn->key = (char *)(malloc(sizeof(char)));
    strcpy(nn->key, key);
    nn->value = value;
    nn->left = NULL;
    nn->right = NULL;
    nn->next = NULL;
    nn->leaf = LEAF;
    if ((*list) == NULL){
        (*list) = nn;
        return;
    }
    ll iter = (*list);
    if (iter->value >= value){
        (*list) = nn;
        nn->next = iter;
        return;
    }
    while (iter->next != NULL && iter->next->value <= value){
        iter = iter->next;
    }
    ll temp = iter->next;
    iter->next = nn;
    nn->next = temp;
    return;
}

void insert_node_using_node_ll(ll* list, tree* nn){
    ll iter = (*list);
    if (iter == NULL){
        iter = (*nn);
        return;
    }
    if (iter->value >= (*nn)->value){
        (*list) = (*nn);
        (*nn)->next = iter;
        return;
    }
    while (iter->next != NULL && iter->next->value <= (*nn)->value){
        iter = iter->next;
    }
    ll temp = iter->next;
    iter->next = (*nn);
    (*nn)->next = temp;
    return;
}

int count_node_ll(ll list){
    int count = 0;
    while (list != NULL){
        count++;
        list = list->next;
    }
    return count;
}

void iterate_ll(ll list){
    while (list != NULL){
        printf("%s : ", list->key);
        printf("%d\n", list->value);
        list = list->next;
    }
}