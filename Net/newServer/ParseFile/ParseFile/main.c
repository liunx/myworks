#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "readfile.h"
#include "getword.h"
#include "config.h"
#define FILENAME        "test.conf"

int main(int argc, const char *argv[])
{
        char *file;
        file = read_file(FILENAME);
        //printf("%s", file);
        parse_file(file);
        printf("The value of total_servers: %s\n", total_servers);
        sleep(100);

        return 0;
}
