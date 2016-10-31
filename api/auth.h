#ifndef AUTH_H
#define AUTH_H

response signup(request req, a_string body);
response handleLogin(request req, a_string body);
response HandleGetProfile(request req, a_string body);

#endif
