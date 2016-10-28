#ifndef COM_H
#define COM_H

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

/*
    getRequest (PUBLIC)
    DESCRTIPTION: get request object
    OUTPUT:
        getRequest
    MEMORY:
        + getRequest
*/
request getRequest();

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
a_string GetBody();

/*
    destroyRequest (PUBLIC)
    DESCRIPTION: destroy the request
    INPUT:
        req - the request
    MEMORY:
        - req
*/
void destroyRequest(request req);

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
response createResponse(int status, a_string statusText, a_string contentType);

/*
    destroyResponse (PUBLIC)
    DESCRIPTION: destroy response
    INPUT:
        res - response
    MEMORY:
        - res
*/
void destroyResponse(response res);

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
void send(response res);

#endif
