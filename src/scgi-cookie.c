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
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "scgi-helpers.h"
#include "scgi-cookie.h"


t_scgi_cookie * scgi_cookie_create(const char *name, const char *value, time_t expire, const char *path, const char *domain, bool secure, bool httponly, bool todelete) {
    t_scgi_cookie *cookie;

    cookie = (t_scgi_cookie *)malloc(sizeof(t_scgi_cookie));
    if (cookie == NULL) {
        return(NULL);
    }

    memset(cookie, 0, sizeof(t_scgi_cookie));
    if (name != NULL) {
        cookie->name = strdup(name);
    }
    if (value != NULL) {
        cookie->value = strdup(value);
    }
    if (path != NULL) {
        cookie->path = strdup(path);
    }
    if (domain != NULL) {
        cookie->domain = strdup(domain);
    }
    cookie->expire = expire;
    cookie->secure = secure;
    cookie->httponly = httponly;
    cookie->todelete = todelete;

    return(cookie);
}


void scgi_cookie_free(t_scgi_cookie *cookie) {

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


char * scgi_cookie_tostring(t_scgi_cookie *cookie) {
    char *encname, *encvalue, *buffer;
    char gmtstr[100];
    time_t t;
    struct tm *tps;
    
    if (cookie == NULL) {
        return(NULL);
    }

    buffer = (char *)malloc(SCGI_COOKIE_SZ);
    if (buffer == NULL) {
        return(NULL);
    }

    encname = scgi_urlencode(cookie->name, strlen(cookie->name));
    if (cookie->value == NULL || cookie->todelete) {
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

    if (cookie->todelete) {
        /* one full day, a past datetime is enough */
        cookie->expire = -(24 * 60 * 60);
    }

    if (cookie->expire != 0) {
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

    if (cookie->httponly) {
        if (strlen(buffer) > SCGI_COOKIE_SZ) {
            free(buffer);
            return(NULL);
        }

        strcat(buffer, "; httponly");
    }

    return(buffer);
}
