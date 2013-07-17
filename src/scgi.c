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
#include <errno.h>

#include "scgi.h"

/* environment variables */
extern char **environ;

static void _scgi_redirect(t_scgi *ctx, const char *absolute_url, bool end, const char *code);

static void scgi_header_clear(t_scgi *ctx);


t_scgi *scgi_init(void) {
    t_scgi *l_cgi = NULL;
    struct scgi_dictionary *enve;
    char **ep, *env, *eq;

    /* alloc and make everything zero'd */
    l_cgi = calloc(1, sizeof(t_scgi));
    if (l_cgi == NULL) {
        return((t_scgi *)NULL);
    }

    TAILQ_INIT(&(l_cgi->headers));
    TAILQ_INIT(&(l_cgi->envs));

    for (ep = environ; *ep; ep++) {
        env = strdup(*ep);
        if (env == NULL) {
            scgi_free(l_cgi);
            return((t_scgi *)NULL);
        }
        enve = (struct scgi_dictionary *)malloc(sizeof(struct scgi_dictionary));
        if (enve == NULL) {
            scgi_free(l_cgi);
            free(env);
            return((t_scgi *)NULL);
        }

        memset(enve, 0, sizeof(struct scgi_dictionary));
        if ((eq = strchr(env, '=')) != NULL) {
            *eq = '\0'; eq++;
            enve->key = strdup(env);
            enve->value = strdup(eq);
            TAILQ_INSERT_TAIL(&(l_cgi->envs), enve, entry);
        }
        free(env);
    }

    scgi_set_content_type(l_cgi, SCGI_TEXT_HTML);
    l_cgi->_outstream = stdout;

    return(l_cgi);
}


void scgi_free(t_scgi *ctx) {
    struct scgi_dictionary *ev, *evn;

    scgi_header_clear(ctx);

    ev = TAILQ_FIRST(&(ctx->envs));
    while (ev != NULL) {
        evn = TAILQ_NEXT(ev, entry);
        if (ev->key) {
            free(ev->key);
        }
        if (ev->value) {
            free(ev->value);
        }
        free(ev);
        ev = evn;
    }
    TAILQ_INIT(&(ctx->envs));

    if (ctx->buffer.buffer != NULL) {
        free(ctx->buffer.buffer);
    }

    free(ctx);
}


char * scgi_envs_lookup(const char *key, t_scgi *ctx) {
    struct scgi_dictionary *ev;
    
    if (key == NULL || *key == '\0') {
        return((char *)NULL);
    }
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

    if (!ctx->_writenHeaders) {
        TAILQ_FOREACH(he, &(ctx->headers), entry) {
            scgi_header_fprint(ctx->_outstream, he->header);
        }
        fprintf(ctx->_outstream, SCGI_EOL);
        ctx->_writenHeaders = true;
    }
}


void scgi_printf(t_scgi *ctx, const char *fmt, ...) {
    va_list ap;
    char *str;
    bool flush;

    flush = false;
    if (ctx->buffered && !ctx->forceflush) {
        va_start(ap, fmt);
        if (vasprintf(&str, fmt, ap) == -1) {
#if defined(DEBUG)
            fprintf(stderr, "%s: %s, at line %d\n", __func__, strerror(errno), __LINE__);
#endif
            va_end(ap);
            if (ctx->exit) {
                ctx->exit(1);
            }
            scgi_free(ctx);
            exit(1);
        }
        va_end(ap);
        if (scgi_buffer_write(&(ctx->buffer), str) != 0) {
            free(str);
            if (ctx->exit) {
                ctx->exit(2);
            }
            scgi_free(ctx);
            exit(2);
        }
        free(str);
    } else {
        scgi_headers_print(ctx);

        if (!ctx->buffer.flushed && ctx->buffer.length != 0) {
            scgi_buffer_flush(&(ctx->buffer), ctx->_outstream);
            flush = true;
        }

        va_start(ap, fmt);
        (void)vfprintf(ctx->_outstream, fmt, ap);
        va_end(ap);
        if (flush) {
            fflush(ctx->_outstream);
        }
    }
}


void scgi_puts(t_scgi *ctx, const char *str) {
    bool flush;

    flush = false;
    if (ctx->buffered && !ctx->forceflush) {
        if (scgi_buffer_write(&(ctx->buffer), str) != 0) {
            if (ctx->exit) {
                ctx->exit(1);
            }
            scgi_free(ctx);
            exit(1);
        }
    } else {
        scgi_headers_print(ctx);

        if (!ctx->buffer.flushed && ctx->buffer.length != 0) {
            scgi_buffer_flush(&(ctx->buffer), ctx->_outstream);
            flush = true;
        }

        fputs(str, ctx->_outstream);
        if (flush) {
            fflush(ctx->_outstream);
        }
    }
}


void scgi_eor(t_scgi *ctx) {
    
    if (ctx->buffered && !ctx->buffer.flushed && ctx->buffer.length != 0) {
        ctx->forceflush = true;
    }
    scgi_printf(ctx, SCGI_EOR);
    scgi_free(ctx);
    exit(0);
}


void scgi_set_content_type(t_scgi *ctx, const char *content_type) {
    t_scgi_header *header;
    struct scgi_header_entry *he;

    header = scgi_headers_lookup(SCGI_CONTENT_TYPE, &(ctx->headers));
    if (header) {
        /* update */
        if (header->data) {
            header->free(header->data);
        }
        scgi_header_ct_set(header, strdup(content_type));
    } else {
        /* create */
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
        /* update : delete cookie + create cookie */
        if (header->data) {
            header->free(header->data);
        }
        cookie = scgi_cookie_create(name, value, expire, path, domain, secure);
        if (cookie != NULL) {
            header->data = cookie;
        }
    } else {
        /* create cookie header */
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


void scgi_redirect(t_scgi *ctx, const char *absolute_url, bool end) {

    _scgi_redirect(ctx, absolute_url, end, SCGI_STATUS_CODE_302);
}


void scgi_redirectpermanent(t_scgi *ctx, const char *absolute_url, bool end) {

    _scgi_redirect(ctx, absolute_url, end, SCGI_STATUS_CODE_301);
}


static void _scgi_redirect(t_scgi *ctx, const char *absolute_url, bool end, const char *code) {
    t_scgi_header *header;
    struct scgi_header_entry *he;
    char *url, *ptr;

    if (absolute_url == NULL || *absolute_url == '\0') {
        return;
    }

    url = strdup(absolute_url);
    if ((ptr = strrchr(url, '\n')) != NULL) {
        *ptr =  '\0';
    }

    scgi_header_clear(ctx);

    /* status */
    header = scgi_header_status_create(code);
    he = (struct scgi_header_entry *)malloc(sizeof(struct scgi_header_entry));
    if (he) {
        he->header = header;
        TAILQ_INSERT_TAIL(&(ctx->headers), he, entry);
    }

    /* location */
    header = scgi_header_location_create(url);
    he = (struct scgi_header_entry *)malloc(sizeof(struct scgi_header_entry));
    if (he) {
        he->header = header;
        TAILQ_INSERT_TAIL(&(ctx->headers), he, entry);
    }

    /* some browsers that can not handle location header */
    scgi_printf(ctx, "<html><head><title>Object moved</title></head><body>");
    scgi_printf(ctx, "<h2>Object moved to <a href=\"%s\">here</a></h2>", url);
    scgi_printf(ctx, "</body><html>");

    if (end) {
        scgi_eor(ctx);
    }
}


static void scgi_header_clear(t_scgi *ctx) {
    struct scgi_header_entry *he, *hen;

    he = TAILQ_FIRST(&(ctx->headers));
    while (he != NULL) {
        hen = TAILQ_NEXT(he, entry);
        if (he->header->data) {
            he->header->free(he->header->data);
        }
        free(he);
        he = hen;
    }
    TAILQ_INIT(&(ctx->headers));
}

