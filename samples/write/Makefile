PROG= scgi-write

CFLAGS+= -I../../include
CFLAGS+= -DLOGO_FULL=\"${.CURDIR}/logo-full.png\"

LDADD+= -L../../src -lscgi

SRCS+= program.c


WITHOUT_PROFILE=    1
WITHOUT_INFO=       1
WITHOUT_DOC=        1
NO_OBJ=             1
NO_MAN=             1
DEBUG_FLAGS=        -g -ggdb
WARNS=              2

.include <bsd.prog.mk>
