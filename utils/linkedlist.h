#pragma once
#ifndef _LINKEDLIST_H
#define _LINKEDLIST_H
#endif

#include "node.h"
typedef node* ll;
#include "tree.h"

void insert_node_ll(ll* list, char* key, int value);
void insert_node_using_node_ll(ll* list, tree* nn);
int count_node_ll(ll list);
void iterate_ll(ll list);