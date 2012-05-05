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

#include "scgi-header-cookie.h"


static char *scgi_cookie_tostring(t_scgi_header *header);
static void scgi_cookie_freedata(void *data);


t_scgi_header * scgi_header_cookie_create(const char *name, const char *value, time_t expire, const char *path, const char *domain, bool secure) {
    t_scgi_cookie *cookie;

    cookie = scgi_cookie_create(name, value, expire, path, domain, secure);
    if (cookie == NULL) {
        return((t_scgi_header *)NULL);
    }

    return(scgi_header_create(SCGI_SET_COOKIE, cookie, scgi_cookie_tostring, scgi_cookie_freedata));
}


void scgi_header_cookie_free(t_scgi_header *header) {

    header->free(header->data);
    free(header);
    header = NULL;
}


t_scgi_header * scgi_header_cookies_lookup(const char *name, struct scgi_headers_head *headers) {
    struct scgi_header_entry *he;

    if (TAILQ_EMPTY(headers)) {
        return((t_scgi_header *)NULL);
    }
    TAILQ_FOREACH(he, headers, entry) {
        if (strcmp(he->header->name, SCGI_SET_COOKIE) == 0
            && strcmp(((t_scgi_cookie *)he->header->data)->name, name) == 0) {
            return(he->header);
        }
    }
    return((t_scgi_header *)NULL);
}


static char * scgi_cookie_tostring(t_scgi_header *header) {
    t_scgi_cookie *cookie;
    char *encname, *encvalue, *buffer;
    char gmtstr[100];
    time_t t;
    struct tm *tps;

    buffer = (char *)malloc(SCGI_COOKIE_SZ);
    if (buffer == NULL) {
        return(NULL);
    }

    cookie = (t_scgi_cookie *)header->data;

    encname = scgi_urlencode(cookie->name, strlen(cookie->name));
    if (cookie->value == NULL) {
        encvalue = strdup("");
    } else {
        encvalue = scgi_urlencode(cookie->value, strlen(cookie->value));
    }
    snprintf(buffer, SCGI_COOKIE_SZ, "%s=%s", encname, encvalue);
    free(encname);
    free(encvalue);

    if (cookie->domain != NULL) {
        if (strchr(cookie->domain, '/') != NULL || strchr(cookie->domain, '.') == NULL) {
            free(buffer);
            return(NULL);
        }
        if (strlen(buffer) > SCGI_COOKIE_SZ) {
            free(buffer);
            return(NULL);
        }

        strcat(buffer, "; domain=");
        strcat(buffer, cookie->domain);
    }

    if (cookie->path != NULL) {
        if (cookie->path[0] != '/') {
            free(buffer);
            return(NULL);
        }
        if (strlen(buffer) > SCGI_COOKIE_SZ) {
            free(buffer);
            return(NULL);
        }

        strcat(buffer, "; path=");
        strcat(buffer, cookie->path);
    }

    if (cookie->expire > 0) {
        t = time(NULL);
        t += cookie->expire;
        tps = gmtime(&t);
        strftime(gmtstr, sizeof(gmtstr), "%a, %d-%b-%Y %H:%M:%S GMT", tps);

        if (strlen(buffer) > (SCGI_COOKIE_SZ - sizeof(gmtstr))) {
            free(buffer);
            return(NULL);
        }

        strcat(buffer, "; expires=");
        strcat(buffer, gmtstr);
    }

    if (cookie->secure) {
        if (strlen(buffer) > SCGI_COOKIE_SZ) {
            free(buffer);
            return(NULL);
        }

        strcat(buffer, "; secure");
    }

    return(buffer);
}


static void scgi_cookie_freedata(void *data) {
    t_scgi_cookie *cookie;

    cookie = (t_scgi_cookie *)data;
    if (cookie->name) {
        free(cookie->name);
    }
    if (cookie->value) {
        free(cookie->value);
    }
    if (cookie->path) {
        free(cookie->path);
    }
    if (cookie->domain) {
        free(cookie->domain);
    }
    free(cookie);
}

