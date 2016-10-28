#ifndef ROUTE_H
#define ROUTE_H

/*
    handleRequest (PUBLIC)
    DESCRIPTION: handle a request
    INPUT:
        req - request
        body - request body
    OUTPUT:
        handleRequest - response
    MEMORY:
        + handleResponse
*/
response handleRequest(request req, a_string body);

#endif
