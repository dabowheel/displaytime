#ifndef COM_H
#define COM_H

typedef struct request {
    a_string request_uri;
    a_string method;
    a_string content_type;
    a_string content_length;
    a_string query_string;
    a_string script_name;
} *request;

typedef struct response {
    int status;
    a_string statusText;
    a_string contentType;
    a_string_builder body;
} *response;

request getRequest();
a_string GetBody();
void destroyRequest(request req);
response createResponse(int status, a_string statusText, a_string contentType);
void destroyResponse(response res);
void send(response res);
response ApplicationError();
response ApplicationErrorDescription(const char *description);
response ApplicationErrorDetails(const char *description, a_string details);
response FormResponse();

#endif
