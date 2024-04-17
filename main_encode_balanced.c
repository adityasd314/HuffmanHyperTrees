#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "hashtable/hashtable.h"
#include "node/node.h"
#include "node/linkedlist.h"
#include "node/tree.h"
#include "node/htree.h"
#include "bitbuffer/bitbuf.h"

char* bit_one = "1";

int main(int argc, char** argv){

    //This piece of code for reading all the alphabets from the file
    //storing the count and its corresponding character in the hashtable.
    ht freqMap;
    hashtable_create(&freqMap);
    int num = 1;
    int characters_before_compression = 0;
    int characters_after_compression = 0;
    FILE *file = fopen(argv[1],"r");
    if (file == NULL){
        printf("No Such File exsists!");
        return 0;
    }
    char* str = (char *)(malloc(sizeof(char)));
    do{
        characters_before_compression++;
        str[0] = fgetc(file);
        if (feof(file)){
            break ;
        }
        ht_set(&freqMap,str,&num,INT_INPUT,0);
    }while(1);
    rewind(file);
    hti iterator = ht_iterator(&freqMap);
    ht_next(&iterator);
    // printf("The number of distinct characters in file : %zu\n",freqMap->length);



    //Adding the elements in the hashtable into a LinkedList and then
    //Creating the Huffman Encoded Tree.
    ll list = NULL;
    ht huffchar_to_code, huffcode_to_char;
    tree t;
    hashtable_create(&huffchar_to_code);
    hashtable_create(&huffcode_to_char);
    int elements = (int)freqMap->length;
    for (int i = 0 ; i < elements ; i++){
        // printf("%s - %d\n",(char *)iterator.key, *(int *)iterator.value);
        insert_node_ll(&list, (char *)iterator.key, *(int *)iterator.value);
        ht_next(&iterator);
    }
    printf("Number of nodes added in the linkedlist are : %d\n",count_node_ll(list)); 
    // iterate_ll(list);
    if (elements == 0){
        printf("Empty Text File !");
        return 0;
    }
    if (elements == 1){
        char *string = "1";
        ht_set(&huffchar_to_code, list->key, string, NON_INT_INPUT,0);
        ht_set(&huffcode_to_char, list->key, string, NON_INT_INPUT,1);
    }
    else{
        insert_nodes_tree_balanced(&t, &list);
        htree_creator(&huffchar_to_code, &huffcode_to_char, &t);
    }
    tree_display_level(t);

    // Getting the length of Max and Min Lengths of the Huffman codes created.
    int min_code_len = INT_MAX;
    int max_code_len = INT_MIN;
    hti iterator_huffman = ht_iterator(&huffchar_to_code);
    int len;
    ht_next(&iterator_huffman);
    FILE *fileptr = fopen("htree_cash_balanced.txt", "w");
    elements = (int)huffchar_to_code->length;
    for (int i = 0 ; i < elements ; i++){
        fprintf(fileptr,"%s %s\n",iterator_huffman.key,(char *)iterator_huffman.value);
        printf("%s : ", iterator_huffman.key);
        printf("%s\n", (char *)iterator_huffman.value);

        len = strlen(iterator_huffman.value);
        if (min_code_len > len){
            min_code_len = len;
        }if (max_code_len < len){
            max_code_len = len;
        }ht_next(&iterator_huffman);
    }
    printf("Maximum length of the codes : %d\n", max_code_len);
    printf("Minimum length of the codes : %d\n", min_code_len);
    printf("Number of elements in the huffman coded Hashtable - %zu\n", huffchar_to_code->length);
    printf("Number of elements in the huffman coded Hashtable - %zu\n", huffcode_to_char->length);



    // Writing the encoded binary file
    FILE* fpw = fopen("random_huffman_balanced","w");
    bitbuf *bw = bitbuf_new_bit_writer(fpw);
    int length = 0, bit = 0;
    char *name;
    do{
        str[0] = fgetc(file);
        if (feof(file)){
            break ;
        }
        name = (char *)ht_get(&huffchar_to_code,str);
        if (name == NULL){
            printf("Huffman code not found for : %s\n",str);
        }
        length = strlen(name);
        for (int i = 0 ; i < length ; i++){
            bit = name[i]-48;
            if (bitbuf_write_bit(bw, bit, &characters_after_compression) == false){
                printf("Error!\n");
            }
        }
    }while(1);
    bitbuf_free(bw, &characters_after_compression);
    printf("Characters before compression are - %d\n", characters_before_compression);
    printf("Characters after compression are - %d\n", characters_after_compression);

    // Decoding the Binary file
    // Make a struct of the array, size and pointer.
    // Decoding using Htree.
    FILE* fp = fopen("random_huffman_balanced","r");
    FILE* decoded_file = fopen("decoded_output_using_htree.txt","w");
    FILE* decoded_file_tree = fopen("decoded_output_using_tree.txt","w");
    bitbuf *br = bitbuf_new_bit_reader(fp);
    char* code = (char *)(malloc(sizeof(char)*max_code_len));
    iterator_huffman = ht_iterator(&huffchar_to_code);
    ht_next(&iterator_huffman);
    int htree_lookup = 0;
    int idx = -1;
    bit = 0;
    int found = 0;
    char* key;
    while (true){
        bit = bitbuf_read_bit(br);
        if (bit == EOF){
            break;
        }
        
        if (bit == 0){
            code[++idx] = '0';
        }else{
            code[++idx] = '1';
        }
        if (strlen(code) < min_code_len){
            continue;
        }
        if ((key = ht_get(&huffcode_to_char, code)) != NULL){
            found = 1;
            fprintf(decoded_file, "%s", key);
        }
        htree_lookup++;
    if (found == 1){
            found = 0;
            idx = -1;
            memset(code, 0, max_code_len);
            // break;
        }
        if (idx > max_code_len){
            printf("Error in decoding!\n");
            break;
        }
    }
    printf("Lookup Operations using Htree - %d\n", htree_lookup);

    //Decoding using Tree
    rewind(fp);
    br = bitbuf_new_bit_reader(fp);
    int tree_lookup = 0;
    tree iterator_tree = t;
    while (true){
        tree_lookup++;
        bit = bitbuf_read_bit(br);
        if (bit == EOF){
            break;
        }

        if (bit == 0){
            iterator_tree = iterator_tree->left;
        }else{
            iterator_tree = iterator_tree->right;
        }

        if (iterator_tree == NULL){
            printf("Error in decoding\n");
            break;
        }

        if (iterator_tree->leaf == LEAF){
            fprintf(decoded_file_tree, "%s", (char *)iterator_tree->key);
            iterator_tree = t;
        }
    }
    printf("Lookup Operations using tree - %d\n", tree_lookup);
    float ans = (float)tree_lookup/(float)htree_lookup;
    printf("The Lookup Reduction Ratio is %0.4f", ans);
}