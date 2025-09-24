# Assignment: Practicing the Process API
Practicing with fork, exec, wait. 

### Overview

In this assignment, you will practice using the Process API to create processes and run programs under Linux. The goal is to gain hands-on experience with system calls related to process management. Specifically, you will practice using the unix process API functions 'fork()', 'exec()', 'wait()', and 'exit()'. 

⚠️ Note: This is not an OS/161 assignment. You will complete it directly on Linux. 

Use the Linux in your CSE4001 container. If you are using macOS, you may use the Terminal (you may need to install development tools with C/C++ compilers). 

**Reference Reading**: Arpaci-Dusseau, *Operating Systems: Three Easy Pieces*, Chapter 5 (Process API Basics)
 👉 [Chapter 5 PDF](http://pages.cs.wisc.edu/~remzi/OSTEP/cpu-api.pdf)

---

### **Steps to Complete the Assignment**

1. **Accept the GitHub Classroom Invitation**
    [GitHub Link](https://classroom.github.com/a/FZh4BrQG)
2. **Set up your Repository**
   - Clone the assignment repository.
3. **Study the Reference Materials**
   - Read **Chapter 5**.
   - Download and explore the sample programs from the textbook repository:
      [OSTEP CPU API Code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/cpu-api).
4. **Write Your Programs**
   - Adapt the provided example code to answer the assignment questions.
   - Each program should be clear, well-commented, and compile/run correctly.
   - Add your solution source code to the repository.

5. **Prepare Your Report**
   - Answer the questions in the README.md file. You must edit the README.md file and not create another file with the answers. 
   - For each question:
     - Include your **code**.
     - Provide your **answer/explanation**.
6. **Submit Your Work via GitHub**
   - Push both your **program code** to your assignment repository.
   - This push will serve as your submission.
   - Make sure all files, answers, and screenshots are uploaded and rendered properly.








---
### Questions
1. Write a program that calls `fork()`. Before calling `fork()`, have the main process access a variable (e.g., x) and set its value to something (e.g., 100). What value is the variable in the child process? What happens to the variable when both the child and parent change the value of x?


```cpp
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


// When a variable is set before calling fork(), it is available in both the parent and child process with the same value.
// If a variable is changed within a process, the changes are not reflected in the other processes.

```


2. Write a program that opens a file (with the `open()` system call) and then calls `fork()` to create a new process. Can both the child and parent access the file descriptor returned by `open()`? What happens when they are writing to the file concurrently, i.e., at the same time?

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    printf("hello, I am main process (pid:%d)\n", (int) getpid());
    int fd = open("q2.txt", O_RDWR);
    printf("calling fork\n");
    int rc = fork();
    if (rc < 0) {
        // fork failed; exit
        fprintf(stderr, "fork failed\n");
        exit(1);
    } else if (rc == 0) {
        // child (new process)
        printf("hello, I am child (pid:%d)\n", (int) getpid());
        dprintf(fd, "I am child process writing to file");
    } else {
        // parent goes down this path (original process)
        printf("hello, I am parent of %d (pid:%d)\n",
               rc, (int) getpid());
        dprintf(fd, "I am parent process writing to file");
    }
    printf("fd is %d\n", fd);
    close(fd);
    return 0;
}

// Both the parent and child file can access the file descriptor.
//When the parent and child processes write to the file at the same time, the parent takes precedence and writes before the child.
```

3. Write another program using `fork()`.The child process should print “hello”; the parent process should print “goodbye”. You should try to ensure that the child process always prints first; can you do this without calling `wait()` in the parent?

```cpp
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
        printf("hello\n");
    } else {
        // parent goes down this path (original process)
        sleep(1);
        printf("goodbye\n");
    }
    return 0;
}

// I used sleep(1) to ensure the parent process always prints after the child.
// This works because both the parent and child process start at the same time, and the child can print a short string in much less than 1 second
```


4. Write a program that calls `fork()` and then calls some form of `exec()` to run the program `/bin/ls`. See if you can try all of the variants of `exec()`, including (on Linux) `execl()`, `execle()`, `execlp()`, `execv()`, `execvp()`, and `execvpe()`. Why do you think there are so many variants of the same basic call?

```cpp
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

// There are likely several variations of exec() for developer convienience, with several ways for C to execute a program based on how specific the execution parameters have to be.
```

5. Now write a program that uses `wait()` to wait for the child process to finish in the parent. What does `wait()` return? What happens if you use `wait()` in the child?

```cpp
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
        printf("child's pid is %d\n", (int) getpid());
        printf("child says hello\n");
    } else {
        // parent goes down this path (original process)
        printf("wait calls %d\n", wait(NULL));
        printf("parent says goodbye\n");
    }
    return 0;
}

// wait() returns the pid of the terminated child process.
// if wait() were used in the child, the child process would wait for the parent process to terminate before continuing.
```

6. Write a slight modification of the previous program, this time using `waitpid()` instead of `wait()`. When would `waitpid()` be useful?

```cpp
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
        printf("child's pid is %d\n", (int) getpid());
        printf("child says hello\n");
    } else {
        // parent goes down this path (original process)
        printf("waitpid() returns %d\n", waitpid(rc, NULL, WCONTINUED));
        printf("parent says goodbye\n");
    }
    return 0;
}

// waitpid() is useful when a process needs to wait for a specific child process to terminate in order to continue, as it can wait for a specific pid to terminate.
```

7. Write a program that creates a child process, and then in the child closes standard output (`STDOUT FILENO`). What happens if the child calls `printf()` to print some output after closing the descriptor?

```cpp
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

// The child closing standard output prevents them from printing any further outputs to the terminal.
// No error is thrown, printf() simply does nothing.
```

