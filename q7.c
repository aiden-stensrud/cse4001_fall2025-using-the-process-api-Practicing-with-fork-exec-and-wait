#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    printf("hello, I am main process (pid:%d)\n", (int) getpid());
    printf("calling fork\n");
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("child process\n");
	close(STDOUT_FILENO);
	printf("i have just closed stdout fileno");
    } else {
        // parent goes down this path (original process)
	printf("parent process\n");
    }
    return 0;
}
