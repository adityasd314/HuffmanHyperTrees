#pragma once
#ifndef _NODE_H
#define _NODE_H
#endif

typedef struct node{
    char *key;
    int value; 
    int leaf;
    struct node* left;
    struct node* right;
    struct node* next;
}node;

#define LEAF 1
#define NO_LEAF 0