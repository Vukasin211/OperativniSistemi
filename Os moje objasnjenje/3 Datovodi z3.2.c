#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
int main()
{
	//Generise random broj u zavistnosti od trenutnog vremena
    srand(time(NULL));

	//Deklarisanje dva datovoda (pipe)
    int pip1[2];
    int pip2[2];

	//Inicijalizacija datovoda
    if (pipe(pip1) < 0)
        return -1;
    if (pipe(pip2) < 0)
        return -1;

	//Forkuje se proces (ovo je u dete procesu)
    if (fork() == 0)
    {
		//Zatvaranje krajeva koji se ne koriste
        close(pip1[0]);
        close(pip2[0]);
        char niz[20];
        int s = 0;
        for (int i = 0; i < 20; i++)
        {
            niz[i] = rand() % 75 + 48;
            s += niz[i];
        }
        printf("%d\n", s);
        if (s % 2 == 0)
        {
            int a = 0;
            int b = 1;
			//slanje podataka kroz datovod
            write(pip1[1], &niz[0], sizeof(char));
            write(pip2[1], 0, sizeof(char));

            for (int i = 1; i < 20; i++)
            {
				//slanje podataka kroz datovod
                write(pip1[1], &niz[i], sizeof(char));
            }
        }
        else
        {
            int a = 0;
            int b = 1;
			//slanje podataka kroz datovod
            write(pip2[1], &niz[0], sizeof(char));
            write(pip1[1], 0, sizeof(char));
            for (int i = 1; i < 20; i++)
            {
				//slanje podataka kroz datovod
                write(pip2[1], &niz[i], sizeof(char));
            }
        }
		//Zatvaranje krajeva nakon slanja
        close(pip1[1]);
        close(pip2[1]);
    }
	//Ovo izvrsava proces roditelj
    else
    {
		//Zatvaranje krajeva koji se ne koriste
        close(pip1[1]);
        close(pip2[1]);
		//Forkovanje procesa (ovo izvrsava dete proces)
        if (fork() == 0)
        {
            close(pip2[0]);
            int s = 0;
            char niz[20];
			//Citanje iz datovoda
            read(pip1[0], &niz[0], sizeof(char));
            if (niz[0] != 0)
            {
                FILE *f = fopen("Karakteri.txt", "w");
				//Citanje iz datovoda
                read(pip1[0], &niz[0], sizeof(char));
                fprintf(f, "%d ", niz[0]);
                for (int i = 1; i < 20; i++)
                {
					//Citanje iz datovoda
                    read(pip1[0], &niz[i], sizeof(char));
                    fprintf(f, "%d ", niz[i]);
                    fflush(f);
                }
                fclose(f);
            }
			//Zatvaranje datovoda nakon citanja
            close(pip1[0]);
        }
		//Forkovanje procesa (ovo je u dete procesu)
        else if (fork() == 0)
        {
			//Zatvaranje kraja koji se ne koristi
            close(pip1[0]);
            int s = 0;
            char niz[20];
            read(pip2[0], &niz[0], sizeof(char));
            if (niz[0] != 0)
            {
                printf("%d \n", niz[0]);
                fflush(stdout);
                for (int i = 1; i < 20; i++)
                {
					//Citanje iz datovoda
                    read(pip2[0], &niz[i], sizeof(char));
                    printf("%d \n", niz[i]);
                    fflush(stdout);
                }
            }
			//Zatvaranje nakon citanja
            close(pip2[0]);
        }
    }
}