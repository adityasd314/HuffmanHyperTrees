htree_encode_balanced:
	cc -c main_encode_balanced.c
	cc -c node/htree.c
	cc -c node/linkedlist.c
	cc -c node/tree.c
	cc -c bitbuffer/bitbuf.c
	cc -c hashtable/hashtable.c
	cc -c queue/queue.c
	cc main_encode_balanced.o tree.o linkedlist.o htree.o hashtable.o bitbuf.o queue.o
htree_encode_unbalanced:
	cc -c main_encode_unbalanced.c
	cc -c node/htree.c
	cc -c node/linkedlist.c
	cc -c node/tree.c
	cc -c bitbuffer/bitbuf.c
	cc -c hashtable/hashtable.c
	cc -c queue/queue.c
	cc main_encode_unbalanced.o tree.o linkedlist.o htree.o hashtable.o bitbuf.o queue.o
clean:
	rm *.o
	rm *.out