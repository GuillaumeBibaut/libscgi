#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scgi-helpers.h"

int main(void) {
    char *decoded, *encoded;

    decoded = strdup("les 3/4 du temps, j'essaie de faire + ! \"intéressant\" ... ? h&h& #;]");
    printf("%s\n", decoded);

    encoded = scgi_urlencode(decoded, strlen(decoded));
    printf("%s\n", encoded);

    (void)scgi_urldecode(encoded);
    printf("%s\n", encoded);

    free(encoded);
    free(decoded);

    return(0);
}

