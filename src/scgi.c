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
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/queue.h>

#include "scgi.h"

extern char **environ;


t_scgi *scgi_init(void) {
    t_scgi *l_cgi = NULL;
    struct scgi_hash *enve;
    char **ep, *env, *eq;

    l_cgi = (t_scgi *)malloc(sizeof(t_scgi));
    if (l_cgi == NULL) {
        return((t_scgi *)NULL);
    }

    memset(l_cgi, 0, sizeof(t_scgi));

    TAILQ_INIT(&(l_cgi->headers));
    TAILQ_INIT(&(l_cgi->envs));

    for (ep = environ; *ep; ep++) {
        env = *ep;
        enve = (struct scgi_hash *)malloc(sizeof(struct scgi_hash));
        if (enve == NULL) {
            continue;
        }

        enve->key = env;
        if ((eq = strchr(env, '=')) != NULL) {
            *eq = '\0'; eq++;
            enve->value = eq;
        }
        TAILQ_INSERT_TAIL(&(l_cgi->envs), enve, entry);
    }

    scgi_set_content_type(l_cgi, SCGI_TEXT_HTML);
    l_cgi->outstream = stdout;
    l_cgi->writen = false;

    return(l_cgi);
}


char * scgi_envs_lookup(const char *key, t_scgi *ctx) {
    struct scgi_hash *ev;
    
    if (TAILQ_EMPTY(&(ctx->envs))) {
        return((char *)NULL);
    }
    TAILQ_FOREACH(ev, &(ctx->envs), entry) {
        if (strcmp(ev->key, key) == 0) {
            return(ev->value);
        }
    }
    return((char *)NULL);
}


void scgi_headers_print(t_scgi *ctx) {
    struct scgi_header_entry *he;

    if (!ctx->writen) {
        TAILQ_FOREACH(he, &(ctx->headers), entry) {
            scgi_header_fprint(ctx->outstream, he->header);
        }
        fprintf(ctx->outstream, SCGI_EOL);
        ctx->writen = true;
    }
}


void scgi_printf(t_scgi *ctx, const char *fmt, ...) {
    va_list ap;

    scgi_headers_print(ctx);

    va_start(ap, fmt);
    (void)vfprintf(ctx->outstream, fmt, ap);
    va_end(ap);
}


void scgi_set_content_type(t_scgi *ctx, const char *content_type) {
    t_scgi_header *header;
    struct scgi_header_entry *he;

    header = scgi_headers_lookup(SCGI_CONTENT_TYPE, &(ctx->headers));
    if (header) {
        if (header->data) {
            header->free(header->data);
        }
        scgi_header_ct_set(header, strdup(content_type));
    } else {
        header = scgi_header_ct_create(content_type);
        he = (struct scgi_header_entry *)malloc(sizeof(struct scgi_header_entry));
        if (he) {
            he->header = header;
            TAILQ_INSERT_TAIL(&(ctx->headers), he, entry);
        }
    }
}


void scgi_set_cookie(t_scgi *ctx, const char *name, const char *value, time_t expire, const char *path, const char *domain, bool secure) {
    t_scgi_header *header;
    t_scgi_cookie *cookie;
    struct scgi_header_entry *he;

    header = scgi_header_cookies_lookup(name, &(ctx->headers));
    if (header) {
        if (header->data) {
            header->free(header->data);
        }
        cookie = scgi_cookie_create(name, value, expire, path, domain, secure);
        if (cookie != NULL) {
            header->data = cookie;
        }
    } else {
        header = scgi_header_cookie_create(name, value, expire, path, domain, secure);
        if (header != NULL) {
            he = (struct scgi_header_entry *)malloc(sizeof(struct scgi_header_entry));
            if (he) {
                he->header = header;
                TAILQ_INSERT_TAIL(&(ctx->headers), he, entry);
            }
        }
    }
}


void scgi_set_cookie_permanent(t_scgi *ctx, const char *name, const char *value, const char *path, const char *domain, bool secure) {

    scgi_set_cookie(ctx, name, value, SCGI_COOKIE_PERMANENT, path, domain, secure);
}


void scgi_clear_cookie(t_scgi *ctx, const char *name, const char *path, const char *domain, bool secure) {

    scgi_set_cookie(ctx, name, NULL, -1, path, domain, secure);
}
