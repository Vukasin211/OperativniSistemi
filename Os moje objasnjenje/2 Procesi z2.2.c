#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//Isto kao Z2.1 samo se koristi System V semafor sto je pozeljnije za sinhronizaciju procesa

int main(int arg, char *argv[])
{
	//ID semafora
    int s1, s2;
	//definisanje Unije za postavljanje vrednosti semafora
    union semun
    {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } sem;

	//Definisanje struktura za zakljucavanje i otkljucavanje semafora
    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock = { 0, 1, 0};

	//Kreiranje dva semafora (s1 i s2 su ID-jevi)
    s1 = semget((__key_t)100, 1, 0666 | IPC_CREAT);
    s2 = semget((__key_t)200, 1, 0666 | IPC_CREAT);

	//Inicijalizacija semafora (ne dodeljulje se direktno nego prvo unija dobija vrednost pa "semctl" setuje vrednost u semafor (s1 i s2))
    sem.val = 1;
    semctl(s1, 0, SETVAL, sem);
    sem.val = 0;
    semctl(s2, 0, SETVAL, sem);

	//Otvaranje fajla
    FILE *f;
    char ime[20] = "fajl.txt";
    //int N = *argv[2] - 48;
    int N = 3;

    //strcpy(ime, "./");
    //strcat(ime, argv[1]);

    f = fopen(ime, "w");
	//Forkuju se procesi
    int n = fork();
	//Ovo izvrsava dete proces
    if (n == 0)
    {
        for (int i = 0; i < N; i++)
        {
			//Zakljucavanje semafora s1
            semop(s1, &sem_lock, 1);
            fprintf(f, "%d ", rand() % 20);
            fprintf(f, "%d ", rand() % 20);
            fprintf(f, "%d //", rand() % 20);
            fflush(f);
			//Otkljucavanje semafora s2
            semop(s2, &sem_unlock, 1);
        }
    }
	//Ovo izvrsava proces roditelj
    else
    {
        for (int i = 0; i < N; i++)
        {
			//Zakljucavanje semafora s2
            semop(s2, &sem_lock, 1);
            fprintf(f, "%d ", (rand() % 20) * (-1));
            fprintf(f, "%d\n", (rand() % 20) * (-1));
            fflush(f);
			//Otkljucavanje semafora s1
            semop(s1, &sem_unlock, 1);
        }
    }

	//Brisanje semafor setova
    semctl(s1, 0, IPC_RMID, 0);
    semctl(s2, 0, IPC_RMID, 0);

	//Zatvaranje fajla
    fclose(f);
    return 0;
}