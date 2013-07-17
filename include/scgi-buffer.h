#ifndef __SCGI_BUFFER_H__
#define __SCGI_BUFFER_H__

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#define SCGI_BUFFER_CAPACITYSZ (16 * 1024U)

typedef struct scgi_buffer t_scgi_buffer;
struct scgi_buffer {
    char *buffer;
    size_t length;
    size_t capacity;
    bool flushed;
};

int scgi_buffer_write(t_scgi_buffer *buffer, const char *str);
void scgi_buffer_flush(t_scgi_buffer *buffer, FILE *outstream);

#endif /* __SCGI_BUFFER_H__ */
