#include <stdio.h>

#include "scgi.h"

int main(int argc, char *argv[]) {
    t_scgi *ctx = NULL;

    ctx = scgi_init();
    scgi_set_content_type(ctx, SCGI_TEXT_PLAIN);

    scgi_envs_print(ctx);

    scgi_printf(ctx, "\nenvs[REMOTE_ADDR] = \"%s\"\n", scgi_envs_lookup("REMOTE_ADDR", ctx));
    scgi_printf(ctx, "envs[REMOTE_PORT] = \"%s\"\n", scgi_envs_lookup("REMOTE_PORT", ctx));
    scgi_printf(ctx, "envs[QUERY_STRING] = \"%s\"\n", scgi_envs_lookup("QUERY_STRING", ctx));
    scgi_printf(ctx, "argv[0] = \"%s\"\n", argv[0]);

    scgi_eor(ctx);

    return(0);
}

