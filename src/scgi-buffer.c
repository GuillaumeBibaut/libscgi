/*-
 * Copyright (c) 2013, Guillaume Bibaut.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of Guillaume Bibaut nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL Guillaume Bibaut BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
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
    
    if (buffer == NULL) {
        return(-1);
    }
    if (str == NULL || *str == '\0') {
        return(0);
    }
    if (buffer->flushed && buffer->length != 0 && buffer->buffer != NULL) {
        free(buffer->buffer);
        buffer->buffer = NULL;
        buffer->length = 0;
        buffer->capacity = 0;
        buffer->flushed = false;
    }
    length = strlen(str);
    capacity = ((((buffer->length + length) / SCGI_BUFFER_CAPACITYSZ) + 1) * SCGI_BUFFER_CAPACITYSZ) + 1;
    if (buffer->buffer == NULL || capacity >= buffer->capacity) {
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
void scgi_buffer_flush(t_scgi_buffer *buffer, FILE *outstream) {

    if (!buffer->flushed && buffer->length != 0) {
        fputs(buffer->buffer, outstream);
        buffer->flushed = true;
    }
}

