/* execlp ile bir proses içinden başka program çalıştırma örneği.
Example to show how to call another program within a process by
using execlp. */


#include <stdio.h>	//  printf
#include <unistd.h>	//  fork, getpid, execlp
#include <stdlib.h>	//  exit

int main(void)
{
  printf("Process ID: %d \n", getpid());
  execlp("./newprog", "newprog", (char *) NULL);
  printf("Original process ... \n");
  exit(0);
  return 0;
}
