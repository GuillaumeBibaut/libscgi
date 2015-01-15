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
#include <stdbool.h>

#include "scgi.h"

#if !defined(LOGO_FULL)
#error "NO LOGO DEFINED!"
#endif

int main(int argc, char *argv[]) {
    FILE *image;
    FILE *fout;
    char *buf;
    off_t size;
    t_scgi *ctx = NULL;

    ctx = scgi_init();
    ctx->withHeaders = false;

    if ((fout = fopen("testout", "w")) == NULL) {
        return(1);
    }
    ctx->_outstream = fout;

    if ((image = fopen(LOGO_FULL, "r")) == NULL) {
        scgi_set_content_type(ctx, "text/plain");
        scgi_puts(ctx, "ERROR");
    } else {
        fseeko(image, 0, SEEK_END);
        size = ftello(image);
        rewind(image);
        buf = calloc(1, size);
        if (buf == NULL) {
            scgi_set_content_type(ctx, "text/plain");
            scgi_puts(ctx, "ALLOC ERROR");
        } else {
            fread(buf, 1, size, image);
            fclose(image);
            scgi_write(ctx, buf, size);
            free(buf);
        }
    }
    scgi_eor(ctx);
    fclose(fout);

    return(0);
}

