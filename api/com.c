#include <aqua/aqua.h>
#include <fcgi_stdio.h>
#include "com.h"
#include "util.h"

/*
    getRequest (PUBLIC)
    DESCRTIPTION: get request object
    OUTPUT:
        getRequest
    MEMORY:
        + getRequest
*/
request getRequest()
{
    /* + req */
    request req = a_malloc(sizeof(struct request));
    req->request_uri = a_cstr2s(getenv("REQUEST_URI"));
    req->method = a_cstr2s(getenv("REQUEST_METHOD"));
    req->content_type = a_cstr2s(getenv("CONTENT_TYPE"));
    req->content_length = a_cstr2s(getenv("CONTENT_LENGTH"));
    req->query_string = a_cstr2s(getenv("QUERY_STRING"));

    /* req | getRequest */
    return req;
}

/*
    GetBody (PUBLIC)
    DESCRIPTION: get the body of the request
    INPUT:
        stdin
    OUTPUT:
        GetBody - the body
    MEMORY:
        + GetBody
*/
a_string GetBody()
{
    int c;
    a_string_builder b;

    /* + b */
    b = a_sbldcreate();
    while ((c = FCGI_getchar()) != EOF) {
        a_sbldaddchar(b, c);
    }

    /* b | GetBody */
    return a_sbld2s(b);
}

/*
    destroyRequest (PUBLIC)
    DESCRIPTION: destroy the request
    INPUT:
        req - the request
    MEMORY:
        - req
*/
void destroyRequest(request req)
{
    /* - req */
    if (req) {
        a_sdestroy(req->request_uri);
        a_sdestroy(req->method);
        a_sdestroy(req->content_type);
        a_sdestroy(req->content_length);
        a_sdestroy(req->query_string);
        free(req);
    }
}

/*
    createResponse (PUBLIC)
    DESCRIPTION: create response object
    INPUT:
        status - status number
        statusText - status text
        contentType - content type
    OUTPUT:
        createResponse - response
    MEMORY:
        + createResponse
*/
response createResponse(int status, a_string statusText, a_string contentType)
{
    /* + res */
    response res = malloc(sizeof(struct response));
    res->status = status;
    res->statusText = statusText;
    res->contentType = contentType;
    res->body = a_sbldcreate();

    /* res | createResponse */
    return res;
}

/*
    destroyResponse (PUBLIC)
    DESCRIPTION: destroy response
    INPUT:
        res - response
    MEMORY:
        - res
*/
void destroyResponse(response res)
{
    /* - res */
    if (res) {
        a_sdestroy(res->statusText);
        a_sdestroy(res->contentType);
        a_sblddestroy(res->body);
        free(res);
    }
}

/*
    send (PUBLIC)
    DESCRIPTION: send response
    INPUT:
        res - response
    OUTPUT:
        res->body set to NULL
        write response
    MEMORY:
        - res->body
*/
void send(response res)
{
    a_string body;
    printf("Status: %d %s\r\n", res->status, res->statusText->data);
    printf("Content-Type: %s\r\n", res->contentType->data);
    printf("Content-Length: %u\r\n", res->body->len);
    printf("\r\n");

    /* - res->body */
    /* + body */
    body = a_sbld2s(res->body);
    res->body = NULL;
    printf(body->data);
    writeLog(body->data);

    /* - body */
    a_sdestroy(body);
}
