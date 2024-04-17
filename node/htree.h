#pragma once
#ifndef _HTREE_H
#define _HTREE_H
#endif
#include "linkedlist.h"
#include "tree.h"
#include "../hashtable/hashtable.h"

typedef struct nodeS{
    int top;
    char* arr;
}nodeS;

typedef nodeS* code;

void htree_creator(ht* huffchar_to_code, ht* huffcode_to_char,  tree* t);
