#ifndef SESSION_H
#define SESSION_H

typedef struct profile {
    char *id;
    char *email;
    char *password;
} *profile;

response HandleGetProfile(request req, a_string body);
response HandleUpdateProfile(request req, a_string body);

#endif
