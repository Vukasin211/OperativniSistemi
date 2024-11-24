#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>

//Sinhronizacija koriscenjem POSIX semafora

int main(int arg, char *argv[])
{
	//Kreiranje semafora
    sem_t *s1;
	//Inicijalizacija semafora na 1 (O_CREAT = ako ne postoji kreira se, 0666 privilegije)
    s1 = sem_open("/sema1", O_CREAT, 0666, 1);
    sem_t *s2;
	//Inicijalizacija semafora na 0 (O_CREAT = ako ne postoji kreira se, 0666 privilegije)
    s2 = sem_open("/sema2", O_CREAT, 0666, 0);
    
    FILE *f;
    char ime[20] = "fajl.txt";
    //int N = *argv[2] - 48;
    int N = 3;

    //strcpy(ime, "./");
    //strcat(ime, argv[1]);

    f = fopen(ime, "w");

	//Forkuje se proces n(piD) je ID child procesa u roditelju dok je u detetu procesu 0
    int n = fork();
	//Nula je u child procesu sto znaci da samo dete izvrsava ovo
    if (n == 0)
    {
        for (int i = 0; i < N; i++)
        {
            sem_wait(s1);
            fprintf(f, "%d ", rand() % 20);
            fprintf(f, "%d ", rand() % 20);
            fprintf(f, "%d //", rand() % 20);
            fflush(f);
            sem_post(s2);
        }
    }
	//Ovo se izvrsava u roditelju
    else
    {
        for (int i = 0; i < N; i++)
        {
            sem_wait(s2);
            fprintf(f, "%d ", (rand() % 20) * (-1));
            fprintf(f, "%d\n", (rand() % 20) * (-1));
            fflush(f);
            sem_post(s1);
        }
    }
	//Zatvaranje semaforas (Semafor se inicijalizuje sa sem_opet, a "deinicijalizuje" sa sem_close)
    sem_close(s1);
    sem_close(s2);
	//Brisanje semafora
    sem_unlink("/sema1");
    sem_unlink("/sema2");
	//Zatvaranje fajla
    fclose(f);
    return 0;
}