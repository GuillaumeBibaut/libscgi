#include <stdio.h>
#include <stdbool.h>

#include "scgi.h"

int main(void) {
    t_scgi *ctx = NULL;

    ctx = scgi_init();

    scgi_set_cookie(ctx, "toto", "titi", 24 * 60 * 60, "/", ".iaelu.net", false);
    scgi_set_cookie(ctx, "toto", "tete", 24 * 60 * 60, "/", ".iaelu.net", false);

    scgi_envs_print(ctx);

    return(0);
}

