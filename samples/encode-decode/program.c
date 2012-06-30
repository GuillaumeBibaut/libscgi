#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scgi.h"

int main(void) {
    t_scgi *ctx;
    char *decoded, *encoded;

    ctx = scgi_init();
    scgi_set_content_type(ctx, SCGI_TEXT_PLAIN);

    decoded = strdup("les 3/4 du temps, j'essaie de faire + ! \"intéressant\" ... ? h&h& #;]");
    scgi_printf(ctx, "%s\n", decoded);

    encoded = scgi_urlencode(decoded, strlen(decoded));
    scgi_printf(ctx, "%s\n", encoded);

    (void)scgi_urldecode(encoded);
    scgi_printf(ctx, "%s\n", encoded);

    free(encoded);
    free(decoded);

    scgi_free(ctx);

    return(0);
}

