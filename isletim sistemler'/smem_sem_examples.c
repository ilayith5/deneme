/* fork - sinyal - pause - semafor - paylasilan bellek bolgesi uygulamasi */

/* derste yapilan ornekte bazi degisiklikler ile 
   farkli senaryolar denenecektir. Tum degisiklikler birbirinden
   bagimsizdir. Yani esas program uzerinden tanimlanmistir. 
   Her birini denerken degisiklikleri eski haline getirmeyi unutmayin.*/

/* DIKKAT!!!!! Denemelerde hatali durumlar test
   ettiginiz icin her denemeden sonra ps ile calisan prosesleri ve ipcs ile
   sistemde bulunan semafor ve paylasilan bellekleri kontrol edin. Prosesler
   sonlanmadiysa kill -9 .... ile elle sonlandirin. Kaynaklardan iade
   edilmeyen varsa ipcrm sem ..... veya ipcrm shm .... ile elle sisteme 
   iade edin. 
*/
/***************************************************************************
1: 
Esas, degistirilmemis programi calistirip, proseslerin calismasini ekrana
cikardiklari mesajlar araciligi ile inceleyin. 

2:
Ana program sonlanmadan once kaynaklari (semaforlar ve paylasilan bellek)
sisteme geri vermesin. ipcs  calistirdiginiz zaman ne gozlemlediniz? Neden?

3:
kilit semaforuna baslangic degeri olarak 0 verin. Ne oldu? Neden?
ps ve ipcs sonuclarini inceleyin. 
(Not: Burada, prosesleri & kullanarak arka planda calistirmanizda fayda var.)

4:
cocuklarin sonda yaptiklari shmdt satirini kaldirin.
cocuklarin sonundaki exit(0) satiri yerine while(1); yazarak 
hic sonlanmamalarini saglayin. ipcs calistirin. 
Farkli olarak ne gordunuz? Neden oldu? ps ile kimliklerini ogrendikten sonra 
kill -9 ... kullanarak cocuklari elle oldurun. Yeniden ipcs calistirdiginizda 
ne degisiklik gozlemlediniz? Neden? 
(Not: Burada, prosesleri & kullanarak arka planda calistirın.)

***********************************************************************/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#define KEYSEM 1
#define KEYSEM2 2
#define KEYSHM 3


void sinyal12(void)
{
}

void sem_signal(int semid, int val)
{
  struct sembuf semafor;

  semafor.sem_num=0;
  semafor.sem_op=val;
  semafor.sem_flg=1;
  semop(semid, &semafor,1);
}

void sem_wait(int semid, int val)
{
  struct sembuf semafor;

  semafor.sem_num=0;
  semafor.sem_op=(-1*val);
  semafor.sem_flg=1;
  semop(semid, &semafor,1);
}

int main (void)
{
  int i, g, f, cocuk[2];
  int sonsem=0,
      kilit=0,
      siram=0,
      pba=0,
      *x=NULL;


  signal(12, (sighandler_t *) sinyal12);

  for (i=0; i<2; i++)
  {
    f=fork();
    if (f==-1)
    {
      printf("FORK hata....\n");
      exit(1);
    }
    if (f==0)
      break;
    cocuk[i]=f;
  }
  if (f!=0)
  {
    sonsem=semget(KEYSEM2, 1, 0700|IPC_CREAT);
    semctl(sonsem, 0, SETVAL,0);

    kilit=semget(KEYSEM,1,0700|IPC_CREAT);
    semctl(kilit,0,SETVAL,1);

    pba=shmget(KEYSHM,sizeof(int),0700|IPC_CREAT);
    x=(int *)shmat(pba,0,0);
    *x=0;
    shmdt(x);
    sleep(2);
    printf("anne kaynaklari yaratti ve cocuklari baslatacak .......\n");
    for (i=0; i<2; i++)
      kill(cocuk[i],12);

    sem_wait(sonsem,2);

    printf("Cocuklarin isi bitti\n");
    semctl(sonsem,0,IPC_RMID,0);
    semctl(kilit,0,IPC_RMID,0);
    shmctl(pba,IPC_RMID,0);
    exit(0);
  }
  else
  {
    siram=i;
    pause();
    kilit=semget(KEYSEM,1,0);
    sonsem=semget(KEYSEM2,1,0);

    pba=shmget(KEYSHM,sizeof(int),0);
    x=(int *) shmat(pba,0,0);

    printf("cocuk %d basliyor ....\n", siram);

    for (i=0; i<5; i++)
    {
        sem_wait(kilit,1);
	printf("   cocuk %d: degeri %d buldum \n", siram, *x);
        g=*x;
	sleep(1);
	g += i;
	*x=g;
	printf("   cocuk %d: yeni degeri %d yaptim \n", siram, *x);
        sem_signal(kilit,1);
	sleep(1);
    }
    shmdt(x);
    sem_signal(sonsem,1);
    exit(0);
  }
  return(0);
}

