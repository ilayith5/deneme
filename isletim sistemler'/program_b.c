#include <stdio.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/sem.h>
# include <sys/shm.h>

#define BASLA_KEY 1
#define KEY_SEMB 2
#define DEVAM_KEY 4
#define KEY_SHM 5

void SemIslem (int semid, int miktar)
{
  struct sembuf a;
  
  a.sem_num=0;
  a.sem_op=miktar;
  a.sem_flg=SEM_UNDO;
  semop(semid, &a, 1); 
}

int main (void)
{
  int basla,
      devam,
      semb,
      shmid, 
      *alan;
/* A prosesi tarafindan yaratilmis olan semaforlarin ve paylasilan bellek
   alaninin tanimlayici degerinin ogrenilmesi. */    
   
  basla=semget(BASLA_KEY, 1, 0);
  devam=semget(DEVAM_KEY, 1, 0);
  semb=semget(KEY_SEMB, 1, 0);
  shmid=shmget(KEY_SHM, 2*sizeof(int), 0700|IPC_CREAT);

/* Paylasilan bellek alaninin (int *) olarak ulasilacak sekilde bellek uzayina
   eklenmesi. */
   
  alan=(int *) shmat(shmid, 0, 0);
  
  SemIslem(basla, -1); /* B prosesinin devam edebilmesi icin basla semaforunun
                          degerinin A prosesi tarafindan arttirilmasi gerekir.*/ 

  while (1)
  {
    SemIslem(semb, -1); /* A prosesi veriyi yerine yazdiktan sonra semb
      	      	      	  semaforunu arttiracak. B prosesi o zamana kadar 
			  bekleyecek. */
    printf(" Proses B: %d\n", alan[0]);
    SemIslem(devam, 1); /* A prosesi yeni veriyi yazmak icin devam semaforunun
                           degerinin artmasini bekliyor. B prosesi veri ile isi 
			   bitince bu degeri bir arttiriyor. */
  }  
    
  return(0);
}   
