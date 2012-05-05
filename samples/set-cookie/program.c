#include <stdio.h>

#include "scgi.h"

int main(void) {
    t_scgi *ctx = NULL;

    ctx = scgi_init();
    scgi_set_content_type(ctx, SCGI_TEXT_PLAIN);

    scgi_envs_print(ctx);

    // TODO

    return(0);
}

