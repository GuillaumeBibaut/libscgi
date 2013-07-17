#include <stdio.h>
#include <unistd.h>

#include "scgi.h"

int main(void) {
    t_scgi *ctx = NULL;

    ctx = scgi_init();
    ctx->buffered = true;


    scgi_puts(ctx, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
    scgi_puts(ctx, "<html xmlns=\"http://www.w3.org/1999/xhtml\" >");
    scgi_puts(ctx, "<head><title>libscgi buffered</title></head>");
    scgi_puts(ctx, "<body><div>buffered sample</div></body>");
    scgi_puts(ctx, "</html>");

    /* Since we are in buffered mode, you HAVE TO tell scgi when it's the end of the response */
    /* mock up 3 sec */
    sleep(3);
    scgi_eor(ctx);
    /* Do not put code after here, it won't happen anyway */

    return(0);
}

