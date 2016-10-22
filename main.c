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
    a_string contentType;
    a_string_builder body;
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

void printHeader()
{
    printf("Content-type: text/html\r\n\r\n");
}

void printRequest(request req)
{
    printf("<p>\n");
    printf("request_uri = %s\n", req->request_uri->data);
    printf("method = %s\n", req->method->data);
    printf("content_type = %s\n", req->content_type->data);
    printf("content_length = %s\n", req->content_length->data);
    printf("query_string = %s\n", req->query_string->data);
    printf("</p>\n");
}

response createResponse(int status, a_string statusText, a_string contentType)
{
    response res = malloc(sizeof(struct response));
    res->status = status;
    res->statusText = statusText;
    res->contentType = contentType;
    res->body = a_sbldcreate();
    return res;
}

void destroyResponse(response res)
{
    if (res) {
        a_sdestroy(res->statusText);
        a_sdestroy(res->contentType);
        a_sblddestroy(res->body);
        free(res);
    }
}

// static int callback(void *NotUsed, int argc, char **argv, char **azColName)
// {
//     return 0;
// }

void send(response res)
{
    a_string body;
    printf("Status: %d %s\r\n", res->status, res->statusText->data);
    printf("Content-Type: %s\r\n\r\n", res->contentType->data);
    body = a_sbld2s(res->body);
    printf(body->data);
    a_sdestroy(body);
}

response signup(request req, a_string body)
{

    response res;
    a_hash_table table;
    a_string email;
    a_string password;

    // a_string_builder query;
    // char *zErrMsg;
    // query = a_sbldcreate();

    sqlite3 *db;
    int rc;
    const char *error;

    rc = sqlite3_open("db", &db);
    if (rc) {
        error = sqlite3_errmsg(db);
        sqlite3_close(db);
        res = createResponse(500, a_cstr2s("Application Error"), a_cstr2s("text/html"));
        a_sbldaddcstr(res->body, "Error opening database: ");
        a_sbldaddcstr(res->body, error);
        send(res);
        return NULL;
    }
    sqlite3_close(db);

    table = a_decodeForm(body);
    email = a_htGet(table, a_cstr2s("email"));
    password = a_htGet(table, a_cstr2s("password"));
    res = createResponse(200, a_cstr2s("OK"), a_cstr2s("text/html"));
    a_sbldaddcstr(res->body, "email = ");
    a_sbldadds(res->body, email);
    a_sbldaddcstr(res->body, "\npassword = ");
    a_sbldadds(res->body, password);
    a_sbldaddcstr(res->body, "\nhello\n");
    send(res);

    return NULL;
}

void handle404(request req)
{
   response res = createResponse(404, a_cstr2s("Not Found"), a_cstr2s("text/html"));
   a_sbldaddcstr(res->body, "Endpoint not found.");
   send(res);
}

response handleRequest(request req, a_string body)
{
    char *path = req->request_uri->data;
    if (strcmp(path, "/api/signup.cgi") == 0) {
        return signup(req, body);
    }
    handle404(req);
    return NULL;
}

 

int main()
{
    while(FCGI_Accept() >= 0) {
        request req = getRequest();
        int c;
        a_string_builder b;
        a_string body;

        b = a_sbldcreate();
        while ((c = FCGI_getchar()) != EOF) {
            a_sbldaddchar(b, c);
        }
        body = a_sbld2s(b);
        handleRequest(req, body);
        destroyRequest(req);
        a_sdestroy(body);
    }

    return 0;
}
