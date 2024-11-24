#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
Korišćenjem programskog jezika C kreirati tri Linux procesa koja komuniciraju korišćenjem datavoda.
Prvi proces generiše niz proizvoljnih celih brojeva(dužina niza je fiksna i iznosi 20 brojeva) a zatim računa zbir svih tako generisanih brojeva.
Ukoliko je dobijeni zbir paran broj, generisani niz brojeva se korišćenjem datavoda šalje drugom procesu koji ih upisuje u datoteku BROJEVI.txt.
Ukoliko je dobijeni zbir neparan broj, generisani niz brojeva se korišćenjem datavoda šalje trećem procesu koji ih štampa na standardnom izlazu.
*/

int main()
{
    srand(time(NULL));

	//Kreiranje indexa za datovode (Datovodima se pristupa preko ID-ja)
    int pip1[2];
    int pip2[2];

	//Kreira se prvi datovod (ako je negativno onda je greska pri kreaciji)
    if (pipe(pip1) < 0)
        return -1;

	//Kreira se drugi datovod (ako je negativno onda je greska pri kreaciji)
    if (pipe(pip2) < 0)
        return -1;

	//Forkovanje procesa i provera pid-a u jednom koraku (Prvo dete proces)
    if (fork() == 0)
    {
		//Zatvaranje datovoda (po jednog kraja)  pretpostavka da se oni ne koriste u ovom procesu vec u roditelju
		//Ovi se ne koriste u ovom procesu (detetu) tako da mogu da se zatvore odmah  
        close(pip1[0]);
        close(pip2[0]);

        int niz[20];
        int s = 0;
        for (int i = 0; i < 20; i++)
        {
            niz[i] = rand() % 100;
            s += niz[i];
        }
        printf("%d\n", s);
        if (s % 2 == 0)
        {
            int a = 0;
            int b = 1;
			//Slanje kroz datovod (pipe)
            write(pip1[1], &b, sizeof(int));
            write(pip2[1], &a, sizeof(int));
            for (int i = 0; i < 20; i++)
            {
                write(pip1[1], &niz[i], sizeof(int));
            }
        }
        else
        {
            int a = 0;
            int b = 1;
            write(pip2[1], &b, sizeof(int));
            write(pip1[1], &a, sizeof(int));
            for (int i = 0; i < 20; i++)
            {
                write(pip2[1], &niz[i], sizeof(int));
            }
        }
		//Zatvaranje krajeva datovoda nakon pisanja u njega
        close(pip1[1]);
        close(pip2[1]);
    }
	//Ovo se izvrsava u procesu roditelju
    else
    {
        close(pip1[1]);
        close(pip2[1]);
		//Drugo dete proces
        if (fork() == 0)
        {
			//Zatvaranje kraja datovoda
            close(pip2[0]);
            int s = 0;
            int niz[20];
            int tmp;
            read(pip2[0], &tmp,sizeof(int));
            if (tmp != 0)
            {                
                FILE *f = fopen("Brojevi.txt", "w");
                for (int i = 0; i < 20; i++)
                {
                    read(pip1[0],&niz[i],sizeof(int));
                    fprintf(f, "%d ", niz[i]);
                    fflush(f);
                }
                fclose(f);
            }
            close(pip1[0]);
        }
        else if (fork() == 0)
        {
            close(pip1[0]);
            int s = 0;
            int niz[20];
            int tmp;
            read(pip2[0], &tmp, sizeof(int));
            if (tmp != 0)
            {
                for (int i = 0; i < 20; i++)
                {
                    read(pip2[0],&niz[i],sizeof(int));
                    printf("%d \n", niz[i]);
                    fflush(stdout);
                }
            }
            close(pip2[0]);
        }
    }
}