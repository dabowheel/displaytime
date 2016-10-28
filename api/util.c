#include <stdio.h>
#include <aqua/aqua.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

FILE *g_fp;

/*
    setupLog (PUBLIC)
*/
void setupLog()
{
  g_fp = fopen("displaytime.log","a");
}

void writeLog(char *message)
{
    time_t utime;
    struct tm *ctime;
    time(&utime);
    ctime = localtime(&utime);
    fprintf(g_fp, "%s %s\n", strtok(asctime(ctime), "\n"), message);
    fflush(g_fp);
}
