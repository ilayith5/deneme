/* fork - signalal - pause - semaphore - shared memory example */

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
  struct sembuf semaphore;

  semaphore.sem_num=0;
  semaphore.sem_op=val;
  semaphore.sem_flg=1;
  semop(semid, &semaphore,1);
}

void sem_wait(int semid, int val)
{
  struct sembuf semaphore;

  semaphore.sem_num=0;
  semaphore.sem_op=(-1*val);
  semaphore.sem_flg=1;
  semop(semid, &semaphore,1);
}

int main (void)
{
  int i, g, myturn, f, children[2];
  int stopsem=0,
      lock=0,
      pba=0,
      *x=NULL;


  signal(12, (sighandler_t) sinyal12);

  for (i=0; i<2; i++)
  {
    f=fork();
    if (f==-1)
    {
      printf("FORK error....\n");
      exit(1);
    }
    if (f==0)
      break;
    children[i]=f;
  }
  if (f!=0)
  {
    printf("Parent: creating resources....\n");
    stopsem=semget(KEYSEM2, 1, 0700|IPC_CREAT);
    semctl(stopsem, 0, SETVAL,0);

    lock=semget(KEYSEM,1,0700|IPC_CREAT);
    semctl(lock,0,SETVAL,1);

    pba=shmget(KEYSHM,sizeof(int),0700|IPC_CREAT);
    x=(int *)shmat(pba,0,0);
    *x=0;
    shmdt(x);
    sleep(2);
    printf("Parent: starting children .......\n");
    for (i=0; i<2; i++)
      kill(children[i],12);

    sem_wait(stopsem,2);

    printf("Parent: Children finished, returning resources...\n");
    semctl(stopsem,0,IPC_RMID,0);
    semctl(lock,0,IPC_RMID,0);
    shmctl(pba,IPC_RMID,0);
    exit(0);
  }
  else
  {
    myturn=i;
    printf("child %d: waiting ....\n", myturn);
    pause();
    lock=semget(KEYSEM,1,0);
    stopsem=semget(KEYSEM2,1,0);
    printf("child %d: heard from parent, beginning work ....\n", myturn);
    pba=shmget(KEYSHM,sizeof(int),0);
    x=(int *) shmat(pba,0,0);

    for (i=0; i<5; i++)
    {
        sem_wait(lock,1);
	printf("   child %d: found %d \n", myturn, *x);
        g=*x;
	sleep(1);
	g += i;
	*x=g;
	printf("   child %d: new value set to %d \n", myturn, *x);
        sem_signal(lock,1);
	sleep(1);
    }

    shmdt(x);
    sem_signal(stopsem,1);
    exit(0);
  }
  return(0);
}
