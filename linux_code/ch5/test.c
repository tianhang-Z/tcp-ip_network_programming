#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define BUFSIZE 30

int main ()
{   

    char buf[BUFSIZE] = {0};

    int len = read(0, buf, BUFSIZE);

    printf("Read buf: %s", buf);
    printf("The length of buf read: %d\n", len);
    printf("The strlen of buf: %ld\n", strlen(buf));

}

