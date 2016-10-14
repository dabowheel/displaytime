#include "fcgi_stdio.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define IMPORT_FROM_AQUA
#include <aqua.h>

extern char **environ;

int main(void)
{
    int count = 0;
    while(FCGI_Accept() >= 0) {
        int c;
        printf("Content-type: text/html\r\n"
                "\r\n"
                "<title>FastCGI Hello!</title>"
                "<h1>FastCGI Hello!</h1>"
                "Request number %d running on host <i>%s</i>\n",
                ++count, getenv("SERVER_NAME"));

        for (char **p = environ; *p; p++) {
            printf("<p>%s</p>\n", *p);
        }

        printf("<p>");        
        while ((c = FCGI_getchar()) != EOF) {
            printf("%c", c);
        }
        printf("</p>\n");
    }
    return 0;
}
