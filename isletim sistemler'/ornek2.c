/* Anne proses 3 çocuk proses yaratıyor. Örnek program, çocukların 
hepsinin pid değerlerinin nasıl tutulduğunu gösteriyor.
Ayrıca global değişkenlerin çocuk proseslere kopyalandığını
gösteriyor. -> i ve t değişkenlerini gözlemleyin. 
3 child processes are created. Program shows how the pid values of
all the children can be saved. It also shows that global variables
are copied to the children -> observe i and t */

#include <stdio.h>	//  printf
#include <unistd.h>	//  fork, getpid
#include <stdlib.h>	//  exit
#include <sys/wait.h>	//   wait
 
int main (void) {
  int i, f, cocuklar[3];
  int t = 10;
  
  for (i=0; i<3; i++) {
    f=fork();
    if (f == -1)  //  fork is not successful
    {
      printf("Error!\n");
      exit(1);
    }

    */ Çocuk prosesler break ile döngüden çıkarak, kendileri de
       yeni çocuk proses yaratmamalılar.
       Child processes must go out of the for loop so that they do not
       also create new children. */
    if (f==0)
       break;

/* yaratılan çocukların pid değerlerinin tutulduğu dizi 
   array holding the pid values of the children */ 
    cocuklar[i]=f;  
  }

  if (f==0) {  // çocuk proses
	printf("    Çocuk proses = %d no= %d t=%d \n", getpid(),i, i+t);
    exit(0);   
  }
  else {
    printf("Anne pid = %d  \n", getpid());    
    //  wait for 5 seconds
    sleep(5);
    printf("\nAnne proses:\n");
    for (i=0; i<3; i++)
      printf("Çocuk[%d] = %d \n", i,cocuklar[i]);
  }
  return (0);
}
