#include "http_parser.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* parse_request(char *request) {

    HttpRequestLine *http_request_line = parse_request_line(request);
    printf("%s", http_request_line->method);
    printf("%s", http_request_line->path);
    printf("%s", http_request_line->version);
}

// TODO: static
HttpRequestLine* parse_request_line(char *request) {
    HttpRequestLine *req = (HttpRequestLine *) malloc(sizeof(HttpRequestLine));
    
    char *start_ptr = request;
    char *end_ptr = strchr(start_ptr, ' ');

    // Method
    size_t length = end_ptr - start_ptr;
    memcpy(req->method, start_ptr, length);
    req->method[length] = '\0';

    // Path
    start_ptr = end_ptr + 1;
    end_ptr = strchr(start_ptr, ' ');
    length = end_ptr - start_ptr;
    req->path = (char *) malloc(length + 1);
    memcpy(req->path, start_ptr, length);
    req->path[length] = '\0';

    // Version
    start_ptr = end_ptr + 1;
    end_ptr = strpbrk(start_ptr, "\r\n");
    length = end_ptr - start_ptr;
    memcpy(req->version, start_ptr, length);
    req->version[length] = '\0';

    return req;
}