#include "htree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* one = "1";
char* zero = "0";

void htree_creator_helper(ht* huffman_codes, ht* huffcode_to_char, tree* t, code* c){
    if ((*t)->leaf == NO_LEAF){
        (*c)->top++;
        (*c)->arr[(*c)->top] = '0';
        // printf("%d\n",(*t)->value);
        htree_creator_helper(huffman_codes, huffcode_to_char,&((*t)->left), c);
        (*c)->top++;
        (*c)->arr[(*c)->top] = '1';
        htree_creator_helper(huffman_codes, huffcode_to_char,&((*t)->right), c);
        (*c)->top--;
        return;
    }
    if ((*t)->leaf == LEAF && ht_get(huffman_codes, (*t)->key) == NULL){
        char *name = (char *)(malloc(sizeof(char)*((*c)->top+1)));
        strncpy(name, (*c)->arr, (*c)->top+1);
        int len = strlen(name);
        // printf("%s - %s - %lu\n", name,  (*t)->key ,strlen(name));
        ht_set(huffman_codes, (*t)->key, (void *)name, len, 0);
        name = (char *)(malloc(sizeof(char)*((*c)->top+1)));
        strncpy(name, (*c)->arr, (*c)->top+1);
        // printf("%s - %s - %lu\n", name,  (*t)->key ,strlen(name));
        ht_set(huffcode_to_char, name, (void *)(*t)->key, (int)strlen(name), 1);
        // printf("Done!\n");
        (*c)->top--;
    }
}

void htree_creator(ht* huffman_codes, ht* huffcode_to_char,  tree* t){
    code c = (code)(malloc(sizeof(nodeS)));
    c->arr = (char *)(malloc(sizeof(char)*10000));
    c->top = -1;
    htree_creator_helper(huffman_codes, huffcode_to_char, t, &c);
}