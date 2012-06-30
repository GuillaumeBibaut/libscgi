/*-
 * Copyright (c) 2012, Guillaume Bibaut.  All rights reserved.
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
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "scgi-defines.h"
#include "scgi-header.h"


t_scgi_header *scgi_header_create(const char *name, void *data, char * (*tostring_func)(t_scgi_header *), void (*free_data_func)(void *)) {
    t_scgi_header *l_header = NULL;

    l_header = (t_scgi_header *)malloc(sizeof(t_scgi_header));
    if (l_header == NULL) {
        return((t_scgi_header *)NULL);
    }

    l_header->name = name;
    l_header->data = data;
    l_header->tostring = tostring_func;
    l_header->free = free_data_func;

    l_header->_writen = false;

    return(l_header);
} 


void scgi_header_fprint(FILE *stream, t_scgi_header *header) {
    char *out_data;

    out_data = header->tostring(header);
    if (out_data && !header->_writen) {
        fprintf(stream, "%s: %s%s", header->name, out_data, SCGI_EOL);
        free(out_data);
        header->_writen = true;
    }
}


t_scgi_header * scgi_headers_lookup(const char *name, struct scgi_headers_head *headers) {
    struct scgi_header_entry *hd;
    
    if (TAILQ_EMPTY(headers)) {
        return((t_scgi_header *)NULL);
    }
    TAILQ_FOREACH(hd, headers, entry) {
        if (strcmp(hd->header->name, name) == 0) {
            return(hd->header);
        }
    }
    return((t_scgi_header *)NULL);
}

