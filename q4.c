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

	char *cmd[] = { "ls", "-l", (char *)0 };
	char *env[] = { "HOME=/usr/home", "LOGNAME=home", (char *)0 };

	// execl()
	execl("/bin/ls", "ls", "-l", (char *)0);
	// execle()
	execle("/bin/ls", "ls", "-l", (char *)0, env);
	// execlp()
	execlp("ls", "ls", "-l", (char *)0);
	// execv()
	execv("/bin/ls", cmd);
	// execve()
	execve("/bin/ls", cmd, env);
	// execvp()
	execvp("ls", cmd);
    } else {
        // parent goes down this path (original process)
	printf("parent process\n");
    }
    return 0;
}
