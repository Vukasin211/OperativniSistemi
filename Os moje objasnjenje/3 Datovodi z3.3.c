#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	//Deklaracija datovoda
    int dat[2];
    int r;

	//Inicijalizacija datovoda
    if (pipe(dat) == -1)
    {
        printf("Greska prilikom kreiranja datavoda!\n");
        return -1;
    }
	//Forkovanje procesa (ovo se izvrsava u roditelju)
    if(fork()!=0){
		//Zatvaranje kraja koji se ne koristi
        close(dat[0]);
        for (int i = 0; i < 50; i++)
        {
            r = rand();
			//Upis u datovod
            write(dat[1],&r,sizeof(r));
        }
        close(dat[1]);
        return 0;
    }

	//Forkovanje procesa (ovo se izvrsava u roditelju)(posto se glavni proces deli na 2 pa se ovde svaki od njih forkuje, 2 roditelja izvrsavaju ovo (ukupno nakon forka ima 4 procesa))
    if(fork()!=0){
		//Zatvaranje kraja koji se ne koristi
        close(dat[1]);
        for (int i = 0; i < 50; i++)
        {
			//Citanje iz datovoda
            read(dat[0],&r,sizeof(r));
            printf("%d ",r);
        }
        close(dat[0]);
        return 0;
    }
}