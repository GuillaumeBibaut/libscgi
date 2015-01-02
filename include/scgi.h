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

#ifndef __SCGI_H__
#define __SCGI_H__

#include <stdio.h>
#include <stdbool.h>
#include <sys/queue.h>

#include "scgi-defines.h"
#include "scgi-helpers.h"
#include "scgi-header.h"
#include "scgi-header-ct.h"
#include "scgi-cookie.h"
#include "scgi-header-cookie.h"
#include "scgi-header-status.h"
#include "scgi-header-location.h"

#include "scgi-buffer.h"

/* Types */

struct scgi_dictionary {
    char *key;
    char *value;

    TAILQ_ENTRY(scgi_dictionary) entry;
};

TAILQ_HEAD(scgi_dictionary_head, scgi_dictionary);

typedef void (*cgiexitfunc)(int err);

typedef struct scgi {

    FILE *_outstream;
    bool _writenHeaders;

    struct scgi_dictionary_head envs;

    struct scgi_headers_head headers;

    bool buffered;
    bool forceflush;
    size_t maxbuffersize;
    t_scgi_buffer buffer;

    cgiexitfunc exit;

} t_scgi;


/* Methods */

t_scgi * scgi_init(void);

void scgi_free(t_scgi *ctx);

char * scgi_envs_lookup(const char *key, t_scgi *ctx);

#define scgi_envs_print(c) \
    do { \
        struct scgi_dictionary *ev; \
        scgi_printf((c), "<pre>\n"); \
        TAILQ_FOREACH(ev, &((c))->envs, entry) { \
            scgi_printf((c), "envs[%s] = \"%s\"\n", ev->key, ev->value); \
        } \
        scgi_printf((c), "</pre>\n"); \
    } while(0)


void scgi_headers_print(t_scgi *ctx);

void scgi_printf(t_scgi *ctx, const char *fmt, ...);

void scgi_puts(t_scgi *ctx, const char *str);

off_t scgi_write(t_scgi *ctx, const char *buf, off_t length);

void scgi_eor(t_scgi *ctx);

/* Header shortcut */

void scgi_set_header(t_scgi *ctx, const char *name, const char *value);

/* Content-Type shortcut */

void scgi_set_content_type(t_scgi *ctx, const char *content_type);

/* Status shortcut */

void scgi_set_status(t_scgi *ctx, const char *code);

/* Location shortcut */

void scgi_set_location(t_scgi *ctx, const char *absolute_url);

/* Cookies shortcuts */

void scgi_set_cookie(t_scgi *ctx, 
    const char *name,
    const char *value,
    time_t expire,
    const char *path,
    const char *domain,
    bool secure);

void scgi_set_cookie_permanent(t_scgi *ctx,
    const char *name,
    const char *value,
    const char *path,
    const char *domain,
    bool secure);

void scgi_clear_cookie(t_scgi *ctx,
    const char *name,
    const char *path,
    const char *domain,
    bool secure);

/* Status + Location shortcut */

void scgi_redirect(t_scgi *ctx, const char *absolute_url, bool end);

void scgi_redirectpermanent(t_scgi *ctx, const char *absolute_url, bool end);

#endif /* __SCGI_H__ */
