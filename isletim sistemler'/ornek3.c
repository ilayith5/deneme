/* fork sistem çağrısı ile yaratılan çocuk proses içinde execlp sistem çağrısı
ile farklı kod çalıştırma örneği.
The child process runs a new program using the execlp system call */

#include <stdio.h>	//  printf
#include <unistd.h>	//  fork, getpid
#include <stdlib.h>	//  exit
#include <sys/wait.h>	//   wait

int main(void)
{
    int f = fork();
    if (f == -1)  //  fork is not successful
    {
        printf("Error!\n");
        exit(1);
    }
    else if (f == 0)  //  child process
    {
        printf("Child: Process ID: %d \n", getpid());
        execlp("./newprog", "newprog", (char *) NULL);
    }
    else  //  parent process
    {
        printf("Parent: Process ID: %d \n", getpid());
        printf("Parent: Terminating... \n");
        exit(0);
    }
    return 0;
}