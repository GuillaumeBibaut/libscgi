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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scgi-header-status.h"

static char * scgi_header_status_tostring(t_scgi_header *header);
static void scgi_header_status_free_data(void *data);



t_scgi_header *scgi_header_status_create(const char *status_code) {

    return(scgi_header_create(SCGI_HEADER_STATUS, scgi_header_status_get_string(status_code), scgi_header_status_tostring, scgi_header_status_free_data));
}


static char * scgi_header_status_tostring(t_scgi_header *header) {

    if (header->data) {
        return(strdup((char *)header->data));
    }
    return(NULL);
}


static void scgi_header_status_free_data(void *data) {
    if (data) {
        free(data);
        data = NULL;
    }
}



#define SCGI_STATUS_CODE_100_STRING "100 Continue"
#define SCGI_STATUS_CODE_101_STRING "101 Switching Protocols"

#define SCGI_STATUS_CODE_200_STRING "200 OK"
#define SCGI_STATUS_CODE_201_STRING "201 Created"
#define SCGI_STATUS_CODE_202_STRING "202 Accepted"
#define SCGI_STATUS_CODE_203_STRING "203 Non-Authoritative Information"
#define SCGI_STATUS_CODE_204_STRING "204 No Content"
#define SCGI_STATUS_CODE_205_STRING "205 Reset Content"
#define SCGI_STATUS_CODE_206_STRING "206 Partial Content"

#define SCGI_STATUS_CODE_300_STRING "300 Multiple Choices"
#define SCGI_STATUS_CODE_301_STRING "301 Moved Permanently"
#define SCGI_STATUS_CODE_302_STRING "302 Found"
#define SCGI_STATUS_CODE_303_STRING "303 See Other"
#define SCGI_STATUS_CODE_304_STRING "304 Not Modified"
#define SCGI_STATUS_CODE_305_STRING "305 Use Proxy"
#define SCGI_STATUS_CODE_306_STRING "306 (Unused)"
#define SCGI_STATUS_CODE_307_STRING "307 Temporary Redirect"

#define SCGI_STATUS_CODE_400_STRING "400 Bad Request"
#define SCGI_STATUS_CODE_401_STRING "401 Unauthorized"
#define SCGI_STATUS_CODE_402_STRING "402 Payment Required"
#define SCGI_STATUS_CODE_403_STRING "403 Forbidden"
#define SCGI_STATUS_CODE_404_STRING "404 Not Found"
#define SCGI_STATUS_CODE_405_STRING "405 Method Not Allowed"
#define SCGI_STATUS_CODE_406_STRING "406 Not Acceptable"
#define SCGI_STATUS_CODE_407_STRING "407 Proxy Authentication Required"
#define SCGI_STATUS_CODE_408_STRING "408 Request Timeout"
#define SCGI_STATUS_CODE_409_STRING "409 Conflict"
#define SCGI_STATUS_CODE_410_STRING "410 Gone"
#define SCGI_STATUS_CODE_411_STRING "411 Length Required"
#define SCGI_STATUS_CODE_412_STRING "412 Precondition Failed"
#define SCGI_STATUS_CODE_413_STRING "413 Request Entity Too Large"
#define SCGI_STATUS_CODE_414_STRING "414 Request-URI Too Long"
#define SCGI_STATUS_CODE_415_STRING "415 Unsupported Media Type"
#define SCGI_STATUS_CODE_416_STRING "416 Request Range Not Satisfiable"
#define SCGI_STATUS_CODE_417_STRING "417 Expectation Failed"

#define SCGI_STATUS_CODE_500_STRING "500 Internal Server Error"
#define SCGI_STATUS_CODE_501_STRING "501 Not Implemented"
#define SCGI_STATUS_CODE_502_STRING "502 Bad Gateway"
#define SCGI_STATUS_CODE_503_STRING "503 Service Unavailable"
#define SCGI_STATUS_CODE_504_STRING "504 Gateway Timeout"
#define SCGI_STATUS_CODE_505_STRING "505 HTTP Version Not Supported"

char *scgi_header_status_get_string(const char *code) {
    char *string;

    if (strcmp(code, SCGI_STATUS_CODE_100) == 0) {
        string = SCGI_STATUS_CODE_100_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_101) == 0) {
        string = SCGI_STATUS_CODE_101_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_200) == 0) {
        string = SCGI_STATUS_CODE_200_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_201) == 0) {
        string = SCGI_STATUS_CODE_201_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_202) == 0) {
        string = SCGI_STATUS_CODE_202_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_203) == 0) {
        string = SCGI_STATUS_CODE_203_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_204) == 0) {
        string = SCGI_STATUS_CODE_204_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_205) == 0) {
        string = SCGI_STATUS_CODE_205_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_206) == 0) {
        string = SCGI_STATUS_CODE_206_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_300) == 0) {
        string = SCGI_STATUS_CODE_300_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_301) == 0) {
        string = SCGI_STATUS_CODE_301_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_302) == 0) {
        string = SCGI_STATUS_CODE_302_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_303) == 0) {
        string = SCGI_STATUS_CODE_303_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_304) == 0) {
        string = SCGI_STATUS_CODE_304_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_305) == 0) {
        string = SCGI_STATUS_CODE_305_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_306) == 0) {
        string = SCGI_STATUS_CODE_306_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_307) == 0) {
        string = SCGI_STATUS_CODE_307_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_400) == 0) {
        string = SCGI_STATUS_CODE_400_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_401) == 0) {
        string = SCGI_STATUS_CODE_401_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_402) == 0) {
        string = SCGI_STATUS_CODE_402_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_403) == 0) {
        string = SCGI_STATUS_CODE_403_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_404) == 0) {
        string = SCGI_STATUS_CODE_404_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_405) == 0) {
        string = SCGI_STATUS_CODE_405_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_406) == 0) {
        string = SCGI_STATUS_CODE_406_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_407) == 0) {
        string = SCGI_STATUS_CODE_407_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_408) == 0) {
        string = SCGI_STATUS_CODE_408_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_409) == 0) {
        string = SCGI_STATUS_CODE_409_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_410) == 0) {
        string = SCGI_STATUS_CODE_410_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_411) == 0) {
        string = SCGI_STATUS_CODE_411_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_412) == 0) {
        string = SCGI_STATUS_CODE_412_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_413) == 0) {
        string = SCGI_STATUS_CODE_413_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_414) == 0) {
        string = SCGI_STATUS_CODE_414_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_415) == 0) {
        string = SCGI_STATUS_CODE_415_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_416) == 0) {
        string = SCGI_STATUS_CODE_416_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_417) == 0) {
        string = SCGI_STATUS_CODE_417_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_500) == 0) {
        string = SCGI_STATUS_CODE_500_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_501) == 0) {
        string = SCGI_STATUS_CODE_501_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_502) == 0) {
        string = SCGI_STATUS_CODE_502_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_503) == 0) {
        string = SCGI_STATUS_CODE_503_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_504) == 0) {
        string = SCGI_STATUS_CODE_504_STRING;
    } else if (strcmp(code, SCGI_STATUS_CODE_505) == 0) {
        string = SCGI_STATUS_CODE_505_STRING;
    } else {
        string = (char *)code;
    }

    return(strdup(string));
}

