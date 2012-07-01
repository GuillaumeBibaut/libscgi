#ifndef __SCGI_DEFINES_H__
#define __SCGI_DEFINES_H__


#define SCGI_EOL "\r\n"
#define SCGI_EOLSZ ((size_t)2)

#define SCGI_EOR SCGI_EOL SCGI_EOL
#define SCGI_EORSZ (SCGI_EOLSZ + SCGI_EOLSZ)


#endif /* __SCGI_DEFINES_H__ */
