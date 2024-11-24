#include <stdio.h>
#include <pthread.h>

//Upisivanje parnih i neparnih brojeva sa 2 threada 
//Sinhronizacija pomocu signala i mutex semafora

//Kreiranje mutex semafora (binarni semafor)
pthread_mutex_t mutex;

//Kreiranje signala 
pthread_cond_t uslov1,uslov2;

void *Neparni(void *a)
{
    FILE *f = (FILE *)a;
	//Zakljucavanje mutexa
    pthread_mutex_lock(&mutex);
    printf("Aloha1\n");
    for (int i = 1; i <= 200; i += 2)
    {
		//Otkljucava mutex i ceka signal, zatim ga ponovo zakljucava nakon primanja signala
        pthread_cond_wait(&uslov1,&mutex);
        printf("AlohaCekanije1\n");
        fprintf(f,"%d ",i);
		//Slanje signala uslov2
        pthread_cond_signal(&uslov2);
        printf("AlohaCekanije11\n");
    }
	//Otkljucavanje mutexa
    pthread_mutex_unlock(&mutex);
    return NULL;
}
void *Parni(void *a)
{
    FILE *f = (FILE *)a;
	//Zakljucavanje mutexa
    pthread_mutex_lock(&mutex);
    printf("Aloha2\n");
    for (int i = 2; i <= 200; i += 2)
    {
		//Otkljucava mutex i ceka signal, zatim ga ponovo zakljucava nakon primanja signala
        pthread_cond_wait(&uslov2,&mutex);
        printf("AlohaCekanije2\n");
        fprintf(f,"%d ",i);
		//Slanje signala uslov1
        pthread_cond_signal(&uslov1);
        printf("AlohaCekanije22\n");
    }
	//Otkljucavanje mutexa
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main()
{
    FILE *f = fopen("z1.2.txt", "w");

	//Kreiranje dve niti
    pthread_t nit1, nit2;

	//Inicijalizacija mutexa (NULL znaci da je default)
    pthread_mutex_init(&mutex,NULL);
	//Inicijalizacija signala
    pthread_cond_init(&uslov1,NULL);
    pthread_cond_init(&uslov2,NULL);

	//Povezivanje niti i funkcija
    pthread_create(&nit1, NULL, Neparni, (void *)f);
    pthread_create(&nit2, NULL, Parni, (void *)f);

    printf("Aloha0\n");

	//Emitovanje signala
    pthread_cond_signal(&uslov1);
    printf("Aloha00\n");

	//Unistavanje niti
    pthread_join(nit1, NULL);
    pthread_join(nit2, NULL);

	//Unistavanje mutexa
    pthread_mutex_destroy(&mutex);
	//Unistavanje signala
    pthread_cond_destroy(&uslov1);
    pthread_cond_destroy(&uslov2);
}
