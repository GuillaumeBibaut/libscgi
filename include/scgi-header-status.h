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
#ifndef __SCGI_HEADER_STATUS_H__
#define __SCGI_HEADER_STATUS_H__

#include "scgi-header.h"

#define SCGI_HEADER_STATUS "Status"

t_scgi_header * scgi_header_status_create(const char *status_code);

char *scgi_header_status_get_string(const char *code);


#define SCGI_STATUS_CODE_100 "100"
#define SCGI_STATUS_CODE_101 "101"

#define SCGI_STATUS_CODE_200 "200"
#define SCGI_STATUS_CODE_201 "201"
#define SCGI_STATUS_CODE_202 "202"
#define SCGI_STATUS_CODE_203 "203"
#define SCGI_STATUS_CODE_204 "204"
#define SCGI_STATUS_CODE_205 "205"
#define SCGI_STATUS_CODE_206 "206"

#define SCGI_STATUS_CODE_300 "300"
#define SCGI_STATUS_CODE_301 "301"
#define SCGI_STATUS_CODE_302 "302"
#define SCGI_STATUS_CODE_303 "303"
#define SCGI_STATUS_CODE_304 "304"
#define SCGI_STATUS_CODE_305 "305"
#define SCGI_STATUS_CODE_306 "306"
#define SCGI_STATUS_CODE_307 "307"

#define SCGI_STATUS_CODE_400 "400"
#define SCGI_STATUS_CODE_401 "401"
#define SCGI_STATUS_CODE_402 "402"
#define SCGI_STATUS_CODE_403 "403"
#define SCGI_STATUS_CODE_404 "404"
#define SCGI_STATUS_CODE_405 "405"
#define SCGI_STATUS_CODE_406 "406"
#define SCGI_STATUS_CODE_407 "407"
#define SCGI_STATUS_CODE_408 "408"
#define SCGI_STATUS_CODE_409 "409"
#define SCGI_STATUS_CODE_410 "410"
#define SCGI_STATUS_CODE_411 "411"
#define SCGI_STATUS_CODE_412 "412"
#define SCGI_STATUS_CODE_413 "413"
#define SCGI_STATUS_CODE_414 "414"
#define SCGI_STATUS_CODE_415 "415"
#define SCGI_STATUS_CODE_416 "416"
#define SCGI_STATUS_CODE_417 "417"

#define SCGI_STATUS_CODE_500 "500"
#define SCGI_STATUS_CODE_501 "501"
#define SCGI_STATUS_CODE_502 "502"
#define SCGI_STATUS_CODE_503 "503"
#define SCGI_STATUS_CODE_504 "504"
#define SCGI_STATUS_CODE_505 "505"

#endif /* __SCGI_HEADER_STATUS_H__ */
