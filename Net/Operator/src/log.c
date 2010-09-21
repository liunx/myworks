/*
 * log-utils -- We can log the operations for 
 * debug.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "log.h"


static void writelog(char buf[], char *s1, char *s2)
{
	int n;
	time_t rawtime;
	char name[32] = {0};
	gethostname(name, sizeof(name));
	time(&rawtime);
	sprintf(buf, "%s", ctime(&rawtime));
	n = strlen(buf);
	sprintf(buf + n - 1, " [%s] %s: %s", name, s1, s2);
}

void logprint(char *s1, char *s2)
{
	int len;
	FILE *fp;
	char buf[1024] = {0};
	fp = fopen(LOGFILE, "a");
	if (fp == NULL)
		return;
	writelog(buf, s1, s2);
	len = strlen(buf);
	fwrite(buf, len, 1, fp);
	fclose(fp);
}

