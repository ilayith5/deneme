/* execlp kullanan örneklerdeki çalıştırılacak düğer kısa kod. 
 This program is executed with execlp */

#include <stdio.h>
#include <unistd.h>	// getpid


int main(void)
{
   printf("Merhaba ben yeni programım. PID= %d \n", getpid());
   return 0;
}
