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

#ifndef __SCGI_HEADER_H__
#define __SCGI_HEADER_H__

#include <stdio.h>
#include <stdbool.h>
#include <sys/queue.h>

/* Type */

typedef struct scgi_header {
    const char *name;
    void *data;

    bool _writen;

    /* a header has to know how to print out */
    char * (*tostring)(struct scgi_header *header);

    /* a header has to know how to free its data memory */
    void (*free)(void *data);
} t_scgi_header;

struct scgi_header_entry {
    t_scgi_header *header;

    TAILQ_ENTRY(scgi_header_entry) entry;
};

TAILQ_HEAD(scgi_headers_head, scgi_header_entry);

/* For instance, if you create a Content-Type header that should be print out :
 * 
 * Content-Type: text/plain
 *
 * then the 'name' of your header has to be "Content-Type"
 */


/* Methods */

t_scgi_header * scgi_header_create(const char *name, void *data, char * (*tostring_func)(t_scgi_header *), void (*free_data_func)(void *));

void scgi_header_fprint(FILE *stream, t_scgi_header *header);
#define scgi_header_print(h) scgi_header_fprint(stdout, h)

t_scgi_header * scgi_headers_lookup(const char *name, struct scgi_headers_head *headers);

#endif /* __SCGI_HEADER_H__ */
