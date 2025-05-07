#include <stdio.h>
/* tum IPC islemleri icin alinmasi gereken baslik dosyalari */
# include <sys/types.h>
# include <sys/ipc.h>

/* semafor islemleri icin alinmasi gereken baslik dosyasi */
# include <sys/sem.h>

/* paylasilan bellek islemleri icin alinmasi gereken baslik dosyasi */
# include <sys/shm.h>

/* signal kullanilmasi icin alinmasi gereken baslik dosyasi */ 
#include <signal.h>

/* sleep kullanilmasi icin alinmasi gereken baslik dosyasi */
#include <unistd.h>

 /* exit kullanilmasi icin alinmasi gereken baslik dosyasi */
#include <stdlib.h>

/* Semaforlar yaratilirken kullanilacak olan anahtar degerlerin tanimlanmasi */
#define BASLA_KEY 1
#define KEY_SEMB 2
#define KEY_SEMC 3
#define DEVAM_KEY 4

/* Paylasilan bellek alani yaratilirken kullanilacak olan anahtar degerinin
   tanimlanmasi */
#define KEY_SHM 5

int basla, /* A prosesi paylasilan kaynaklari yarattiktan sonra bu semaforun
             degerini iki arttirarak, B ve C proseslerinin islerine 
	     baslamalarina izin vermis olacak */
    semb,  /* A prosesi paylasilan bellek alaninin ilk gozune bir deger
      	      yazdiktan sonra bu semaforun degerini bir arttirarak B prosesinin
	      bu degeri okumasina izin vermis olacak. */ 
    semc,  /* A prosesi paylasilan bellek alaninin ikinci gozune bir deger
      	      yazdiktan sonra bu semaforun degerini bir arttirarak C prosesinin
	      bu degeri okumasina izin vermis olacak. */ 
    devam, /* B ve C prosesleri bu semaforun degerini birer arttirarak yazilan
      	      degeri okuduklarini belirtecek ve A prosesinin yeni degerler 
	      yazmasina izin  vermis olacak/ */
    shmid, 
    *alan;  
  
/*****************************************************************************/
/*  SemIslem fonksiyonu kendisine semid parametresi ile bildirilen semafor   */
/*  uzerinde artirma veya eksiltme semafor islemlerini gercekler. Artirma    */
/*  veya  eksiltmenin miktari ise miktar giris parametresi ile verilir.      */
/*****************************************************************************/

void SemIslem (int semid, int miktar)
{
  struct sembuf a;
  
  a.sem_num=0;
  a.sem_op=miktar;
  a.sem_flg=SEM_UNDO;
  semop(semid, &a, 1); 
}

/*****************************************************************************/
/*  Temizle fonksiyonu daha onceden alinmis olan semafor ve paylasilan bellek*/
/*  alanini sisteme iade eder.                                               */
/*****************************************************************************/

void temizle (void)
{
  shmdt(alan); 
  semctl(basla,0,IPC_RMID,0);
  semctl(devam,0,IPC_RMID,0);
  semctl(semb,0,IPC_RMID,0);    
  semctl(semc,0,IPC_RMID,0);
  shmctl(shmid,IPC_RMID,0);
  exit (0);
}

int main (void)
{
  int i;
  
  signal(12, (sighandler_t) temizle); /* 2 numarali sinyal geldiginde temizle
                                  fonksiyonu calisacak. */
  
/* Semaforlarin yaratilmasi ve baslangic degerlerinin atanmasi kismi. 
   Hepsi senkronizasyon amacli kullanildiklarindan, bu programda baslangic 
   degeri olarak 0 verilmelidir. */  
   
  basla=semget(BASLA_KEY, 1, 0700|IPC_CREAT);
  semctl(basla,0,SETVAL,0);
  
  devam=semget(DEVAM_KEY, 1, 0700|IPC_CREAT);
  semctl(devam,0,SETVAL,0);

  
  semb=semget(KEY_SEMB, 1, 0700|IPC_CREAT);
  semctl(semb,0,SETVAL,0);

  semc=semget(KEY_SEMC, 1, 0700|IPC_CREAT);
  semctl(semc,0,SETVAL,0);

/* 2 tamsayi uzunlugunda paylasilan bellek alani yaratilmis ve yaratilan alana 
   bir tamsayi isaretcisi ile ulasilmasi gerektiginden, donen deger 
  (int *) olacak sekilde zorlanmistir. */
  
  shmid=shmget(KEY_SHM, 2*sizeof(int), 0700|IPC_CREAT);
  alan=(int *) shmat(shmid, 0, 0);
  
/* B ve C prosesleri islerina baslayabilmek icin basla semaforunu eksiltmeye
   calisiyorlar. Ancak semaforun baslangic degeri 0 oldugundan artana kadar
   eksiltme satirinda takiliyorlar. A prosesi ise ortak kaynaklari yarattiktan
   sonra basla semaforunun degerini 2 arttiriyor. Bu gerceklestigi anda ise
   B ve C prosesleri takildiklari yerden kurtulup, calismalarina
   baslayacaklardir. */ 
 
  printf("Ortak kaynaklarin yaratilmasi tamamlandi....\n");
  SemIslem(basla, 2);
  
  i=0;
  while (1)
  {
    alan[0]= i;       	/* Ortak alanin ilk gozune deger yazildi. */
    SemIslem(semb, 1);	/* semb semaforunun degeri 1 arttirildi. B prosesi bu
                           semaforun degerini 1 eksiltmeye calismisti ancak 
			   deger 0 oldugundan burada takilmis bekliyordu. 
			   Okunacak veri hazir oldugundan A prosesi degeri 1
			   arttirdiginda B prosesi devam edip degeri okuyor.*/
			   
    alan[1]= 2*i;      /* Ortak alanin ilk gozune deger yazildi. */
    SemIslem(semc, 1); /* semc semaforunun degeri 1 arttirildi. C prosesi bu
                          semaforun degerini 1 eksiltmeye calismisti ancak 
			  deger 0 oldugundan burada takilmis bekliyordu. 
			  Okunacak veri hazir oldugundan A prosesi degeri 1
			  arttirdiginda C prosesi devam edip degeri okuyor.*/
    i++;
    SemIslem(devam, -2); /* A prosesi devam semaforunun degerini iki eksiltmeye
                            calisiyor. B ve C prosesleri veriyi okuyup ekrana
			    cikardiktan sonra bu semaforun degerini 1 arttiracak
			    ve bu durumda A prosesi yeni verileri alana koymak
			    uzere devam edecek. Bu sayede verilerin B ve C
			    prosesleri tarafindan okunamadan yerine baska veri
			    yazilmasi ve veri kaybi olmasi onlenir.*/
    sleep(2);       /* Ekrana cikan yazilari kolay izleyebilmek icin konmus
                       gecikme satiri. */
  }  
    
  return(0);
}   
