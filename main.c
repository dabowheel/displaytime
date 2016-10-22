#include "fcgi_stdio.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <aqua/aqua.h>
#include <sqlite3.h>
#include <string.h>

typedef struct request {
    a_string request_uri;
    a_string method;
    a_string content_type;
    a_string content_length;
    a_string query_string;
} *request;

typedef struct response {
    int status;
    a_string statusText;
    a_string body;
} *response;

request getRequest()
{
    request req = a_malloc(sizeof(struct request));
    req->request_uri = a_cstr2s(getenv("REQUEST_URI"));
    req->method = a_cstr2s(getenv("REQUEST_METHOD"));
    req->content_type = a_cstr2s(getenv("CONTENT_TYPE"));
    req->content_length = a_cstr2s(getenv("CONTENT_LENGTH"));
    req->query_string = a_cstr2s(getenv("QUERY_STRING"));
    return req;
}

void destroyRequest(request req)
{
    if (req) {
        a_sdestroy(req->request_uri);
        a_sdestroy(req->method);
        a_sdestroy(req->content_type);
        a_sdestroy(req->content_length);
        a_sdestroy(req->query_string);
        free(req);
    }
}

response createResponse(int status, a_string statusText, a_string body)
{
    response res = malloc(sizeof(struct response));
    res->statusText = statusText;
    res->body = body;
    return res;
}

void destroyResponse(response res)
{
    if (res) {
        a_sdestroy(res->statusText);
        a_sdestroy(res->body);
        free(res);
    }
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    return 0;
}

response signup(request req, a_string body)
{
    sqlite3 *db;
    int rc;
    a_string_builder b;
    b = a_sbldcreate();
    a_sbldaddcstr(b, "INSERT INTO user VALUES (\"");
    char *zErrMsg;

    rc = sqlite3_open("db", &db);
    if (rc) {
        sqlite3_close(db);
        return createResponse(500, a_cstr2s("Application Error"), a_cstr2s("Cannot open database"));
    }

    rc = sqlite3_exec(db, "", callback, 0, &zErrMsg);
    return NULL;
}

response handleRequest(request req, a_string body)
{
    char *path = req->request_uri->data;
    if (strcmp(path, "/api/signup.cgi")) {
        return signup(req, body);
    }

    return createResponse(404, a_cstr2s("Not Found"), a_cstr2s("Endpoint not found."));
}

int main(void)
{

    while(FCGI_Accept() >= 0) {
        request req = getRequest();
        int c;
        a_string_builder b;
        a_string body;

        printf("Content-type: text/html\r\n\r\n");
        printf("<p>\n");
        printf("request_uri = %s\n", req->request_uri->data);
        printf("method = %s\n", req->method->data);
        printf("content_type = %s\n", req->content_type->data);
        printf("content_length = %s\n", req->content_length->data);
        printf("query_string = %s\n", req->query_string->data);
        printf("</p>\n");

        b = a_sbldcreate();
        while ((c = FCGI_getchar()) != EOF) {
            a_sbldaddchar(b, c);
        }

        body = a_sbld2s(b);
        printf("<p>%s</p>\n", body->data);

        destroyRequest(req);
        a_sdestroy(body);
    }

    return 0;
}
