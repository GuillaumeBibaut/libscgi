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
#include <time.h>

#include "scgi-cookie.h"
#include "scgi-header-cookie.h"

static void scgi_header_cookie_free(void *data);
static char * scgi_header_cookie_tostring(t_scgi_header *header);


t_scgi_header * scgi_header_cookie_create(const char *name,
    const char *value,
    time_t expire,
    const char *path,
    const char *domain,
    bool secure) {
    t_scgi_cookie *cookie;

    cookie = scgi_cookie_create(name, value, expire, path, domain, secure);
    if (cookie == NULL) {
        return((t_scgi_header *)NULL);
    }

    return(scgi_header_create(SCGI_SET_COOKIE, cookie, scgi_header_cookie_tostring, scgi_header_cookie_free));
}


void scgi_header_cookie_free(void *data) {
    
    if (data) {
        scgi_cookie_free((t_scgi_cookie *)data);
        data = NULL;
    }
}


t_scgi_header * scgi_header_cookies_lookup(const char *name, struct scgi_headers_head *headers) {
    struct scgi_header_entry *he;
    t_scgi_cookie *cookie;

    if (TAILQ_EMPTY(headers)) {
        return((t_scgi_header *)NULL);
    }
    TAILQ_FOREACH(he, headers, entry) {
        if (strcmp(he->header->name, SCGI_SET_COOKIE) == 0) {
            cookie = (t_scgi_cookie *)(he->header->data);
            if (strcmp(cookie->name, name) == 0) {
                return(he->header);
            }
        }
    }
    return((t_scgi_header *)NULL);
}


char * scgi_header_cookie_tostring(t_scgi_header *header) {
    return(scgi_cookie_tostring((t_scgi_cookie *)header->data));
}
