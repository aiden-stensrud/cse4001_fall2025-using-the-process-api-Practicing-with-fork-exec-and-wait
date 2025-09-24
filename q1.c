#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int x = 100;
    int y = 100;
    printf("hello, I am main process (pid:%d)\n", (int) getpid());
    printf("my static int x is %d\n", x);
    printf("my changing int y is %d\n", y);
    printf("calling fork\n");
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
	y = 200;
	printf("changing y to %d\n", y);
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
	       rc, (int) getpid());
	y = 300;
	printf("changing y to %d\n", y);
    }
    printf("x is %d\n", x);
    printf("y is %d\n", y);
    return 0;
}
