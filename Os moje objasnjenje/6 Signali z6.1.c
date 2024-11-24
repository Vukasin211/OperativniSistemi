/*
Korišćenjem programskog jezika C napisati UNIX / Linux program koji može da se prekine samo ako se kombinacija tastera Ctrl - C pritisne pet puta za redom.
Ukoliko korisnik pritisne kombinaciju tastera Ctrl - Z dva puta zaredom program na ekranu ispisuje koliko puta do sada je pritisnuto Ctrl - C.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int cc = 0, cz = 0;
//Prekida program kad se ctrl + c pritisne 5 puta
void ctrlc(int a)
{
    signal(SIGINT, ctrlc);
    cz = 0;
    if (++cc == 5)
        exit(0);
}

//Prikazuje broj ctrl + c pritisaka kad se pritisne ctrl + z
void ctrlz(int z)
{
    signal(SIGTSTP, ctrlz);
    if (++cz == 2)
    {
        printf("\"Control+c\" pritisnuto %d puta.\n", cc);
        cc = 0;
    }
}
int main()
{
    signal(SIGINT, ctrlc);
    signal(SIGTSTP, ctrlz);
    while (1)
        pause();
}
