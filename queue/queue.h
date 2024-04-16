#include<stdbool.h>
#include "../node/tree.h"

typedef struct qNode{
    tree data;
    struct qNode* next;
}qNode;

typedef qNode* queue;

void queue_init(queue* q);
void queue_push(queue* q, tree t);
bool queue_empty(queue q);
int queue_size(queue q);
void queue_pop(queue* q);
tree queue_front(queue q);
