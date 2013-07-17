#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

#include "scgi-buffer.h"


/*
 *
 */
int scgi_buffer_write(t_scgi_buffer *buffer, const char *str) {
    size_t length;
    size_t capacity;

    if (buffer->flushed && buffer->length != 0 && buffer->buffer != NULL) {
        free(buffer->buffer);
        buffer->buffer = NULL;
        buffer->length = 0;
        buffer->capacity = 0;
        buffer->flushed = false;
    }
    length = strlen(str);
    capacity = ((((buffer->length + length) / SCGI_BUFFER_CAPACITYSZ) + 1) * SCGI_BUFFER_CAPACITYSZ) + 1;
    if (capacity >= buffer->capacity) {
        buffer->buffer = realloc(buffer->buffer, sizeof(char) * capacity);
        if (buffer->buffer == NULL) {
#if defined(DEBUG)
            fprintf(stderr, "%s: %s, at line %d\n", __func__, strerror(errno), __LINE__);
#endif
            return(-1);
        }
        buffer->capacity = capacity;
    }
    memcpy((buffer->buffer + buffer->length), str, length);
    buffer->length += length;
    buffer->buffer[buffer->length] = '\0';

    return(0);
}


/*
 *
 */
void scgi_buffer_flush(t_scgi_buffer *buffer, FILE *outstream, bool chunked) {

    if (!buffer->flushed && buffer->length != 0) {
        fputs(buffer->buffer, outstream);
        buffer->flushed = true;
    }
}

