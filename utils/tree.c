#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tree.h"
#include "linkedlist.h"
#include "../queue/queue.h"

int height(tree t){
    if (t == NULL){
        return 0;
    }
    int ans = 0;
    int lh = height(t->left);
    int rh = height(t->right);
    if (lh >= rh){
        ans = lh+1;
    }else{
        ans = rh+1;
    }
    return ans;
}

void LL_rotation(tree *root ,tree *t){
    tree z = (*t);
    tree y = (*t)->left;
    tree y_left = y->right;
    (*root) = y;
    y->right = z;
    z->left = y_left; 
    int temp = y->value;
    y->value = z->value;
    z->value = temp;
}

void RR_rotation(tree *root ,tree *t){
    tree z = (*t);
    tree y = (*t)->right;
    tree y_right = y->left;
    (*root) = y;
    y->left = z;
    z->right = y_right;
    int temp = y->value;
    y->value = z->value;
    z->value = temp;
}

void adjust_Rotations(tree *root ,tree *t){
    if ((*t) == NULL){
        return;
    }
    int balance_factor = height((*t)->left) - height((*t)->right);
    if (balance_factor >= 2 || balance_factor <= -2){
        if (balance_factor >= 2){
            LL_rotation(t, t);
        }else{
            RR_rotation(t, t);
        }
        adjust_Rotations(&((*t)->left) ,&((*t)->left));
        adjust_Rotations(&((*t)->right) ,&((*t)->right));
    }
}


//TODO: To handle the cases of single input and no input in text outside.
void insert_nodes_tree(tree* t, ll* l){
    (*t) = NULL;
    int count;
    count = count_node_ll((*l));
    if (count == 0){
        return;
    }else if (count == 1){
        (*t) = (*l);
        return;
    }
    while ((*l) != NULL && (*l)->next != NULL){
        tree temp = (tree)(malloc(sizeof(node)));
        temp->key = NULL;
        temp->leaf = NO_LEAF;
        temp->next = NULL;
        temp->left = (*l);
        temp->right = (*l)->next;
        (*t) = temp;
        (*l) = (*l)->next->next;
        temp->left->next = NULL;
        temp->right->next = NULL;
        temp->value = temp->left->value + temp->right->value;
        // adjust_Rotations(t ,t);
        insert_node_using_node_ll(l,t);
    }
}

void insert_nodes_tree_balanced(tree* t, ll* l){
    (*t) = NULL;
    int count;
    count = count_node_ll((*l));
    if (count == 0){
        return;
    }else if (count == 1){
        (*t) = (*l);
        return;
    }
    while ((*l) != NULL && (*l)->next != NULL){
        tree temp = (tree)(malloc(sizeof(node)));
        temp->key = NULL;
        temp->leaf = NO_LEAF;
        temp->next = NULL;
        temp->left = (*l);
        temp->right = (*l)->next;
        (*t) = temp;
        (*l) = (*l)->next->next;
        temp->left->next = NULL;
        temp->right->next = NULL;
        temp->value = temp->left->value + temp->right->value;
        //Balancing the Tree
        adjust_Rotations(t ,t);
        insert_node_using_node_ll(l, t);
    }
    
}

void print_all_leafs(tree t){
    if (t == NULL){
        return;
    }
    print_all_leafs(t->left);
    print_all_leafs(t->right);
    if (t->leaf == LEAF){
        printf("%s : ",t->key);
        printf("%d\n", (t->value));
    }
}

void tree_display_level(tree t){
    queue q;
    queue_init(&q);
    if (t != NULL){
        queue_push(&q,t);
    }
    int level = 0;
    while (queue_empty(q) == false){
        int n = queue_size(q);
        printf("Level %d : ", level);
        for (int i = 0 ; i < n ; i++){
            tree node = queue_front(q);
            queue_pop(&q);

            if (node->left != NULL){
                queue_push(&q,(node->left));
            }if (node->right != NULL){
                queue_push(&q,(node->right));
            }

            printf("|| %s %d %d %d ||",node->key, node->value, node->leaf, height(node->left)-height(node->right));
        }
        level++;
        printf("\n");
    }
}