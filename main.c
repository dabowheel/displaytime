#include "fcgi_stdio.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define IMPORT_FROM_AQUA
#include <aqua.h>

typedef struct request {
    string request_uri;
    string method;
    string content_type;
    string content_length;
    string query_string;
} *request;

request getRequest()
{
    request req = malloc(sizeof(struct request));
    req->request_uri = cstr2s(getenv("REQUEST_URI"));
    req->method = cstr2s(getenv("REQUEST_METHOD"));
    req->content_type = cstr2s(getenv("CONTENT_TYPE"));
    req->content_length = cstr2s(getenv("CONTENT_LENGTH"));
    req->query_string = cstr2s(getenv("QUERY_STRING"));
    return req;
}

void destroyRequest(request req)
{
    if (req) {
        sdestroy(req->request_uri);
        sdestroy(req->method);
        sdestroy(req->content_type);
        sdestroy(req->content_length);
        sdestroy(req->query_string);
        free(req);
    }
}

int main(void)
{

    while(FCGI_Accept() >= 0) {
        request req = getRequest();
        int c;
        string_builder b;
        string body;

        printf("Content-type: text/html\r\n\r\n");
        printf("<p>\n");
        printf("request_uri = %s\n", req->request_uri->data);
        printf("method = %s\n", req->method->data);
        printf("content_type = %s\n", req->content_type->data);
        printf("content_length = %s\n", req->content_length->data);
        printf("query_string = %s\n", req->query_string->data);
        printf("</p>\n");

        b = sbldcreate();
        while ((c = FCGI_getchar()) != EOF) {
            sbldaddchar(b, c);
        }

        body = sbld2s(b);
        printf("<p>%s</p>\n", body->data);

        destroyRequest(req);
        sdestroy(body);
    }

    return 0;
}
