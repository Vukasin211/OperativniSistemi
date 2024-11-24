#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

//Upisivanje u fajl parnih i neparnih brojeva koriscenjem tredova.

//Globalne promenljive POSIX semafori
sem_t semP, semN;

void *Neparni(void *a)
{
    FILE *f = (FILE *)a;
    for (int i = 1; i <= 20; i += 2)
    {
		//Zakljucavanje semafora (dekrementiranje)
        sem_wait(&semN);
        fprintf(f,"%d ",i);
		//Otkljucavanje semafora (inkrementiranje)
        sem_post(&semP);
    }
    return NULL;
}
void *Parni(void *a)
{
    FILE *f = (FILE *)a;
    for (int i = 2; i <= 20; i += 2)
    {
		//Zakljucavanje semafora (dekrementiranje)
        sem_wait(&semP);
        fprintf(f,"%d ",i);
		//Otkljucavanje semafora (inkrementiranje)
        sem_post(&semN);
    }
    return NULL;
}

int main()
{
	//Otvaranje fajla
    FILE *f = fopen("z1.1.txt", "w");

	//Kreiranje tredova
    pthread_t nit1, nit2;

	//Obicno je 0 u sredini kad se radi multi thread, a kad je multi proces moze i 1 (kad ?)
	//Inicijalizacija semafora semP na 0
    sem_init(&semP, 0, 0);
	//Inicijalizacija semafora semN na 1
    sem_init(&semN, 0, 1);

	//Povezuju se niti i funkcije koje izvrsavaju
    pthread_create(&nit1, NULL, Neparni, (void *)f);
    pthread_create(&nit2, NULL, Parni, (void *)f);

	//Ceka se zavrsetak izvrsenja funkcija za svaki thread
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

	//Unistavanje semafora
    sem_destroy(&semP);
    sem_destroy(&semN);
}