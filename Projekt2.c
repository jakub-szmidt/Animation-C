#include <stdio.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#define ILOSC_OKIEN 2
#define ROZMIAR_SLOWA 20
#define ROZMIAR_OKNA_X(a) (a-4)/2
#define ROZMIAR_OKNA_Y(a) a-3

typedef struct Struktura
{
	int obecna_wspolrzedna_x;
	int obecna_wspolrzedna_y;
	int poprzednia_wspolrzedna_x;
	int poprzednia_wspolrzedna_y;
	char symbol;
}Obiekt;
int dlugosc_slowa(char slowo[ROZMIAR_SLOWA])
{
	int dlugosc=0;
	for (int i=0; i<ROZMIAR_SLOWA; i++)
	{
		if(slowo[i]!=' ' && slowo[i]!='\0')
		{
			dlugosc++;
		}
	}
	return dlugosc;
}
void init_ncurses(int *i, int *j)
{
	initscr();
	keypad(stdscr, TRUE);
	raw();
	getmaxyx(stdscr, *i, *j);
	curs_set(0);
	cbreak();
}
int main()
{
    int wiersze, kolumny, wybor_konca_programu, zakres, licznik_komunikatow=0, czas=1, dlugosc;
    double liczba_obrotow_petli=0;
    char *tytuly[3] = {"PROJEKT NR 2", "INTERFEJS UZYTKOWNIKA", "CO SIE DZIEJE W MIEDZYCZASIE"}, slowo[ROZMIAR_SLOWA] = "Jakub Szmidt";
    WINDOW *okno[ILOSC_OKIEN];
    int (*wskaznik_do_dlugosc_slowa)(char*);
    void (*wskaznik_do_init_ncurses)(int* , int*);
    wskaznik_do_init_ncurses = init_ncurses;
    wskaznik_do_dlugosc_slowa = dlugosc_slowa;
    
    wskaznik_do_init_ncurses(&wiersze, &kolumny);
    dlugosc = wskaznik_do_dlugosc_slowa(slowo);
	
	Obiekt *obiekt;
	obiekt = (Obiekt *)malloc(sizeof(Obiekt));
	
    okno[0] = newwin(ROZMIAR_OKNA_Y(wiersze), ROZMIAR_OKNA_X(kolumny), 2, 2);
    okno[1] = newwin(ROZMIAR_OKNA_Y(wiersze), ROZMIAR_OKNA_X(kolumny), 2, 2+ROZMIAR_OKNA_X(kolumny));
	mvprintw(0, (kolumny-strlen(tytuly[0]))/2, "%s", tytuly[0]);
	mvprintw(1, (kolumny-strlen(tytuly[1]))/4, "%s", tytuly[1]);
	mvprintw(1, ((kolumny-strlen(tytuly[2]))/4)+kolumny/2, "%s", tytuly[2]);
	mvprintw(wiersze-1, 2, "CTRL + C - koniec programu");

    for (int i=0; i<ILOSC_OKIEN; i++)
    {
		box(okno[i], 0, 0);
	}

    obiekt->obecna_wspolrzedna_x = (ROZMIAR_OKNA_X(kolumny))/2;
    obiekt->obecna_wspolrzedna_y = (ROZMIAR_OKNA_Y(wiersze))/2;
    obiekt->poprzednia_wspolrzedna_x = 0;
	obiekt->poprzednia_wspolrzedna_y = 0;
	refresh();
	wrefresh(okno[0]);
	wrefresh(okno[1]);
	
	mvwprintw(okno[0], 1, 1, "Podaj symbol, jaki chcesz aby poruszal sie po funkcji parametrycznej: ");
	wscanw(okno[0], "%c", &obiekt->symbol);
	mvwprintw(okno[1], 1, 1, "Wybrany symbol: %c", obiekt->symbol);
	wclear(okno[0]);
	wrefresh(okno[0]);
	wrefresh(okno[1]);
	
	do
    {
		wclear(okno[0]);
		box(okno[0], 0, 0);
        mvwprintw(okno[0],1,1, "Jaki warunek ukonczenia dzialania progamu wybierasz *:");
        mvwprintw(okno[0],2,1, "1. Okreslony czas");
        mvwprintw(okno[0],3,1, "2. Znikniecie wszystkich literek");
        mvwprintw(okno[0],4,1, "Twoj wybor: ");
        mvwprintw(okno[0], ROZMIAR_OKNA_Y(wiersze)-2, 1, "* Jesli obiekt wyjdzie poza zakres to program zakonczy sie");
        mvwscanw(okno[0],4,13, "%d", &wybor_konca_programu);
        if(wybor_konca_programu!=1 && wybor_konca_programu!=2)
        {
            usleep(500000);
			wrefresh(okno[0]);
        }
        if(wybor_konca_programu == 1)
        {
            mvwprintw(okno[0],5,1, "Podaj jak dlugo ma poruszac sie obiekt (jednostka czasu: 1 jedn. = 1 obrot petli): ");
            wscanw(okno[0], "%d", &zakres);
            wclear(okno[0]);
			wrefresh(okno[0]);
			box(okno[0], 0, 0);
        }
    }
    while(wybor_konca_programu!=1 && wybor_konca_programu!=2);
    
		while((obiekt->obecna_wspolrzedna_y >= 0 && obiekt->obecna_wspolrzedna_y <= ROZMIAR_OKNA_Y(wiersze) && obiekt->obecna_wspolrzedna_x >= 0 && obiekt->obecna_wspolrzedna_x <= ROZMIAR_OKNA_X(kolumny)))
		{
			wclear(okno[0]);
			mvwprintw(okno[0], (ROZMIAR_OKNA_Y(wiersze))/2, ROZMIAR_OKNA_X(kolumny)/2, "%s", slowo);
			obiekt->poprzednia_wspolrzedna_x = obiekt->obecna_wspolrzedna_x;
			obiekt->poprzednia_wspolrzedna_y = obiekt->obecna_wspolrzedna_y;
			
			obiekt->obecna_wspolrzedna_y = 0.1*(liczba_obrotow_petli * sin(liczba_obrotow_petli)) + (ROZMIAR_OKNA_Y(wiersze))/2;
			obiekt->obecna_wspolrzedna_x = 0.1*(liczba_obrotow_petli * cos(liczba_obrotow_petli)) + (ROZMIAR_OKNA_X(kolumny))/2;
			
			if(obiekt->obecna_wspolrzedna_y == (ROZMIAR_OKNA_Y(wiersze))/2 && obiekt->obecna_wspolrzedna_x >= (ROZMIAR_OKNA_X(kolumny))/2 && obiekt->obecna_wspolrzedna_x <= ((ROZMIAR_OKNA_X(kolumny))/2)+strlen(slowo))
			{
				if(slowo[obiekt->obecna_wspolrzedna_x - (ROZMIAR_OKNA_X(kolumny)/2)] != ' ' && slowo[obiekt->obecna_wspolrzedna_x - (ROZMIAR_OKNA_X(kolumny)/2)] != '\0')
				{
					licznik_komunikatow++;
					mvwprintw(okno[1], 4+licznik_komunikatow, 1, "[%d o.p.] Znikniecie literki %c", czas, slowo[obiekt->obecna_wspolrzedna_x - (ROZMIAR_OKNA_X(kolumny)/2)]);
				}
				slowo[obiekt->obecna_wspolrzedna_x - (ROZMIAR_OKNA_X(kolumny)/2)] = ' ';
			}
			mvwprintw(okno[0], obiekt->obecna_wspolrzedna_y, obiekt->obecna_wspolrzedna_x, "%c", obiekt->symbol);
			
			mvwprintw(okno[1], 2, 1, "Obecna wspolrzedna x: %d", obiekt->obecna_wspolrzedna_x);
			mvwprintw(okno[1], 3, 1, "Obecna wspolrzedna y: %d", obiekt->obecna_wspolrzedna_y);
				
			usleep(100000);
	
			liczba_obrotow_petli++;
			box(okno[0], 0, 0);	
			wrefresh(okno[0]);
			wrefresh(okno[1]);
			if((wybor_konca_programu == 1 && czas == zakres) || (wybor_konca_programu == 2 && licznik_komunikatow == dlugosc))
			{
				break;
			}
			czas++;
		}
	mvwprintw(okno[1], 5+licznik_komunikatow, 1, "[%d o.p.] Koniec programu! Kliknij enter aby kontynuowac...", czas);
	wrefresh(okno[1]);
	wrefresh(okno[0]);
	getch();
	delwin(okno[0]);
	delwin(okno[1]);
	endwin();
    return 0;
}
