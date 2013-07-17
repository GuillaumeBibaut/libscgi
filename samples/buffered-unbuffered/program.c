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
    scgi_puts(ctx, "<body>");
    scgi_puts(ctx, "<div>buffered sample part (sleeping 2 seconds to mock up)</div>");

    sleep(2);
    ctx->buffered = false;
    scgi_puts(ctx, "<div>(flushed buffer, sleep 2 seconds again)</div>");
    sleep(2);

    scgi_puts(ctx, "<div>unbuffered sample</div>");
    scgi_puts(ctx, "</body>");
    scgi_puts(ctx, "</html>");

    scgi_eor(ctx);

    return(0);
}

