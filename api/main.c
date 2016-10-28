#include <fcgi_stdio.h>
#include <aqua/aqua.h>
#include "com.h"
#include "route.h"
#include "util.h"

int main()
{
    request req;
    response res;
    a_string body;

    setupLog();
    writeLog("start app");

    while(FCGI_Accept() >= 0) {
        req = getRequest();
        body = GetBody();
        res = handleRequest(req, body);
        send(res);
        destroyRequest(req);
        a_sdestroy(body);
        destroyResponse(res);
    }

    return 0;
}
