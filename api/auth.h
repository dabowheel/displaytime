#ifndef AUTH_H
#define AUTH_H

/*
    signup (PUBLIC)
    DESCRIPTION: handle signup request
    INPUT:
        req - request info
        body - request body
    OUTPUT:
        signup - response
    MEMORY:
        + signup
*/
response signup(request req, a_string body);

/*
    handleLogin (PRIVATE)
    handle login request
    INPUT:
        req - the request
        body - the request body
    OUTPUT:
        handleLogin - response
    MEMORY:
        (+ handleLogin)
*/
response handleLogin(request req, a_string body);

#endif
