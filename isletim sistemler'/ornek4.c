/* wait(NULL) satırı kaldırılarak ve sleep(5) ile bekleme eklenerek 
anne prosesin çocuğundan önce sonlanması sağlandı. Programın
amacı, annesi sonlanan çocuk prosesin yeni annesinin hangi proses 
olduğunu incelemek. 

wait(NULL) line is removed and sleep(5) is added. This allows 
the parent process to terminate before the child. After the 5 seconds
sleep, the child prints its mothers's pid again.
*/

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
        printf("    Child: Process ID: %d \n", getpid());
        printf("    Child: Parent Process ID: %d \n", getppid());
        //  wait for 5 seconds
        sleep(5);
        printf("    Child: Parent Process ID /after 5 secs: %d \n", getppid());
        
    }
    else  //  parent process
    {
        printf("Parent: Process ID: %d \n", getpid());
        printf("Parent: Child Process ID: %d \n", f);
        printf("Parent: Parent Process ID: %d \n", getppid());
        // wait until child process has exited
//      wait(NULL);
        printf("Parent: Terminating... \n");
        exit(0);
    }
    return 0;
}
