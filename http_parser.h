#ifndef HTTP_PARSER_H
#define HTTP_PARSER_H

typedef struct
{
    char method[8];
    char *path;
    char version[12];
} HttpRequestLine;

HttpRequestLine* parse_request_line(char *request);
char* parse_request(char *request);

#endif