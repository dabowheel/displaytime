#include <aqua/aqua.h>
#include <string.h>
#include "com.h"
#include "auth.h"
#include "session.h"
#include "util.h"

/*
    handleNotFound (PRIVATE)
    DESCRIPTION: handle endpoint that doesn't exist
    OUTPUT:
        handleNotFound - the response
    MEMORY:
        + handleNotFound
*/
response handleNotFound()
{
    a_string statusText;
    a_string contentType;
    response res;

    /* + statusText, contentType */
    statusText = a_cstr2s("Not Found");
    contentType = a_cstr2s("text/html");

    /* statusText, contentType | res */
    /* + res */
    res = createResponse(404, statusText, contentType);
    a_sbldaddcstr(res->body, "Endpoint not found.");

    /* - res */
    return res;
}

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
response handleRequest(request req, a_string body)
{
    char *path = req->script_name->data;
    char *method = req->method->data;
    response res;

    writeLog(path);

    /* + res */
    if (strcmp(path, "/api/signup.cgi") == 0) {
        res = signup(req, body);
    } else if (strcmp(path, "/api/login.cgi") == 0) {
        res = handleLogin(req, body);
    } else if (strcmp(path, "/api/profile.cgi") == 0 && strcmp(method, "GET") == 0) {
        res = HandleGetProfile(req, body);
    } else {
        res = handleNotFound(req);
    }

    /* - res */
    return res;
}
