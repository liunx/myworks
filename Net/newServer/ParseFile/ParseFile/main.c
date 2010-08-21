#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "readfile.h"
#include "getword.h"

#define FILENAME        "test.conf"

int main()
{
        char *file;
        file = read_file(FILENAME);
        //printf("%s", file);
        parse_file(file);
        sleep(100);

        return 0;
}
