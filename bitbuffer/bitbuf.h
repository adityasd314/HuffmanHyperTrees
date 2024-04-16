#ifndef BITBUF_HEADER
#define BITBUF_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//#define NDEBUG
#include <assert.h>

#define BITS_IN_BUF     8

typedef struct bitbuf
{
        FILE *fp;
        bool is_writer;
        unsigned char buf;
        unsigned int idx;
}bitbuf;

/**
 * bitbuf_new:  allocates a new bitbuf and returns its pointer.
 *
 *              If writer is true, it will be usable for writing bits to fp,
 *              otherwise for reading bits from fp.
 *              writer must be setted according to the opening mode of fp ("w",
 *              "a" -> true, "r" -> false).
 *
 *              Read and write ("w+", "r+", "a+") modes are a bit tricky to
 *              handle so they are not "officially" supported.
 *
 *              On alloc failure, returns NULL.
 */
bitbuf *bitbuf_new(FILE *fp, bool writer);

#define bitbuf_new_bit_writer(fp)       bitbuf_new((fp), true)

#define bitbuf_new_bit_reader(fp)       bitbuf_new((fp), false)

/**
 * bitbuf_write_bit:  writes a bit (1 if bit == true, 0 if bit == false) and
 *                    returns true.
 *
 *                    Bits are buffered inside a byte from left to right. Only
 *                    with a successful call to bitbuf_flush() they will be
 *                    written to the underlying FILE pointer. This happens
 *                    automatically when the buffer (byte) if full (or when
 *                    bitbuf_free() is called).
 *
 *                    On flush failure, returns false.
 */
bool bitbuf_write_bit(bitbuf *bit_writer, int bit, int* count);

/**
 * bitbuf_flush:  flushes the buffer (a byte) to the underlying FILE pointer and
 *                returns true.
 *
 *                If the buffer is not full, it will be written "0 padded".
 *                If the buffer is empty, nothing will be written and true will
 *                be returned.
 *
 *                On write failure, returns false.
 */
bool bitbuf_flush(bitbuf *bit_writer, int* count);

/**
 * bitbuf_read_bit:  reads a bit and returns 0 or 1 accordingly.
 *
 *                   Bits are buffered inside a byte and read from left to
 *                   right. They will be loaded automatically from the
 *                   underlying FILE pointer when the buffer is empty.
 *
 *                   On load failure, returns EOF.
 */
int bitbuf_read_bit(bitbuf *bit_reader);

/**
 * bitbuf_load:  reads a byte from the underlying FILE pointer, sets it as new
 *               buffer to read from (with bitbuf_read_bit()) and returns true.
 *
 *               On read failure (or EOF), returns false.
 */
bool bitbuf_load(bitbuf *bit_reader);

/**
 * bitbuf_free:  frees memory occupied by bitbuf. If bitbuf is a "bit_writer",
 *               the buffer will be flushed using bitbuf_flush().
 *
 *               It's safe to pass a NULL pointer.
 */
void bitbuf_free(bitbuf *bitbuf, int* count);

/**
 * bitbuf_close:  frees bitbuf (using bitbuf_free()) and calls fclose() on the
 *                underlying FILE pointer used by bitbuf.
 *
 *                It's safe to pass a NULL pointer.
 */
void bitbuf_close(bitbuf *bitbuf, int *count);

#endif
