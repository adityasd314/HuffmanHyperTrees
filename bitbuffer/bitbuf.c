#include "bitbuf.h"


bitbuf *bitbuf_new(FILE *fp, bool writer){
        bitbuf *new_bb = malloc(sizeof(*new_bb));
        if (new_bb == NULL) {
                return NULL;
        }
        new_bb->fp = fp;
        new_bb->is_writer = writer;
        new_bb->buf = 0;
        new_bb->idx = (writer) ? 0 : BITS_IN_BUF;
        return new_bb;
}

bool bitbuf_write_bit(bitbuf *bw, int bit, int* count){
        assert(bw->is_writer);

        if (bw->idx == BITS_IN_BUF && ! bitbuf_flush(bw, count)) {
                return false;
        }
        if (bit != 0) {
                bw->buf |= (0x80 >> bw->idx);
        }
        bw->idx++;
        return true;
}

bool bitbuf_flush(bitbuf *bw, int* count){
        assert(bw->is_writer);

        if (bw->idx > 0) {
                (*count)++;
                if (fputc(bw->buf, bw->fp) == EOF) {
                        return false;
                }
                bw->buf = 0;
                bw->idx = 0;
        }
        return true;
}

int bitbuf_read_bit(bitbuf *br){
        assert( ! br->is_writer);

        if (br->idx == BITS_IN_BUF && ! bitbuf_load(br)) {
                return EOF;
        }
        return ((0x80 >> br->idx++) & br->buf) == 0 ? 0 : 1;
}

bool bitbuf_load(bitbuf *br){
        assert( ! br->is_writer);

        int byte = fgetc(br->fp);
        if (byte == EOF) {
                return false;
        }
        br->buf = byte;
        br->idx = 0;
        return true;
}

void bitbuf_free(bitbuf *bitbuf, int* count){
        if (bitbuf == NULL) {
                return;
        }
        if (bitbuf->is_writer) {
                bitbuf_flush(bitbuf, count);
        }
        free(bitbuf);
}

void bitbuf_close(bitbuf *bitbuf, int *count){
        if (bitbuf == NULL) {
                return;
        }
        FILE *fp = bitbuf->fp;
        bitbuf_free(bitbuf, count);
        fclose(fp);
}