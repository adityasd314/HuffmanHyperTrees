#include "bitbuf.h"

#define N 1000

void test_write_bit(struct bitbuf *bw, int n)
{
        srand(n);
        for (int i = 0; i < n; i++) {
                bitbuf_write_bit(bw, 0);
        }
}

void test_read_bit(struct bitbuf *br, int n)
{
        srand(n);
        for (int i = 0; i < n; i++) {
                assert(bitbuf_read_bit(br) == (rand() % 2 == 0) ? 1 : 0);
        }
}

int main(void)
{
        FILE *fp = fopen("test_sequence_file", "w");
        if (fp == NULL) {
                perror("Error");
                return 1;
        }

        bitbuf *bw = bitbuf_new_bit_writer(fp);
        test_write_bit(bw, N);

        // bitbuf_free(bw);
        // freopen(NULL, "r", fp);

        // struct bitbuf *br = bitbuf_new_bit_reader(fp);
        // test_read_bit(br, N);

        // bitbuf_close(br);

        puts("Test OK");
}