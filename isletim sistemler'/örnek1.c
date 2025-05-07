/* fork sistem çağrısının kullanımını gösteren ilk basit 
örnek. Tek bir çocuk proses yaratılıp, anne ve çocuk 
prosesler bir takım mesajlar bastırıyorlar. 
Simple example to show the use of fork system call. One new process
is created. Both parent and child processes print some messages. */

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
        printf("Child: Parent Process ID: %d \n", getppid());
        //  wait for 10 seconds
        sleep(10);
    }
    else  //  parent process
    {
        printf("Parent: Process ID: %d \n", getpid());
        printf("Parent: Child Process ID: %d \n", f);
        printf("Parent: Parent Process ID: %d \n", getppid());
        //  wait until child process has exited
        wait(NULL);
        printf("Parent: Terminating... \n");
        exit(0);
    }
    return 0;
}
