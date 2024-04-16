#pragma once
#ifndef _TREE_H
#define _TREE_H
#endif

#include "node.h"
typedef node* tree;
#include "linkedlist.h"

void insert_nodes_tree(tree* t, ll* l);
void insert_nodes_tree_balanced(tree* t, ll* l);
void print_all_leafs(tree t);
void tree_display_level(tree t);