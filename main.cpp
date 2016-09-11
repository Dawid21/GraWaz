#include <iostream>
#include <time.h>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <queue>

#define WARUNEK_ZBICIA if (tab[ax][ay]=='o' || ax==0 || ax==x-1 || ay==0 || ay==y-1) goto wyjscie

using namespace std;

void dodaj(char t[][31], int x2, int y2);
void clear( queue<int> &q );
void losuj (char t[][31], int x, int y);
void wypisz(int x, int y, char t[][31]);
int menu (int ilosc_gier=0), menu2();
int celx, cely, punkty;

int main()
{
    int predkosc, x=15, y=31;
    if (!(predkosc=menu())) return 0;

    do
    {
        punkty=-1;
        int ax = x/2, ay = y/2+1;
        char tab [15][31]={};
        tab[0][0]='/'; tab[x-1][y-1]='/'; tab[0][y-1]='\\'; tab[x-1][0]='\\';
        for (int i=1; i<y-1; i++)   {tab[0][i]='=';  tab[x-1][i]='=';}                    // krawedzie mapy - gora, dol
        for (int i=1; i<x-1; i++)   {tab[i][0]='|';  tab[i][y-1]='|';}                    // krawedzie mapy - lewa, prawa
        tab[ax][ay]='x';
        losuj(tab, x, y);                                                                 // losuje polozenie 1. celu
        dodaj(tab, ax, ay-2);
        wypisz(x, y, tab);                                                        // wypisanie mapy i weza

        char znak='d', znak_por=znak;
    while(1)
    {
        int timer = predkosc ;
        while( timer> 0 )
        {
            if(kbhit())
            {
                znak_por= getch();                                              // pobranie znaku sterujacego
            }
            Sleep( 5 ); timer--;
        }
        if (znak_por=='d' && znak!='a') znak=znak_por;
        if (znak_por=='a' && znak!='d') znak=znak_por;
        if (znak_por=='w' && znak!='s') znak=znak_por;
        if (znak_por=='s' && znak!='w') znak=znak_por;

        switch (znak)                                                                 // switch sterujacy
        {
            case 'w':
                { ax--;   WARUNEK_ZBICIA; tab[ax][ay]='x';  dodaj(tab, ax+1, ay); break;}
            case 's':
                { ax++;   WARUNEK_ZBICIA; tab[ax][ay]='x';  dodaj(tab, ax-1, ay); break;}
            case 'a':
                { ay-=2;  WARUNEK_ZBICIA; tab[ax][ay]='x';  dodaj(tab, ax, ay+2); break;}
            case 'd':
                { ay+=2;  WARUNEK_ZBICIA; tab[ax][ay]='x';  dodaj(tab, ax, ay-2); break;}
            default: break;
        }

        if (tab[ax][ay]==tab[celx][cely]) { punkty++; losuj (tab, x, y ); }            // losowanie  celu i punkt

        wypisz (x, y, tab);                                                    // wypisanie mapy i weza
    }
    wyjscie:
    system( "cls" );
    cout << "Przegrales. Liczba zdobytych punktow: " << punkty << endl;
    }while ((predkosc=menu2()));
    return 0;
}

void wypisz(int x, int y, char t[][31])
{
    system( "cls" );
    cout <<"Gra zostala skompilowana: " << __DATE__ << " o godzinie: " << __TIME__ << endl;
    cout << "Liczba punktow: " << punkty << endl;

    for (int i=0; i<x; i++)
        {
            for (int j=0; j<y; j++)     {cout << t[i][j];}
            cout << endl;
        }
}

void dodaj(char t[][31], int x2, int y2)                               // dodanie liczb do kolejki
{
    t[x2][y2]='o';

    static int ile=1;
    static queue < int > dane1;
    static queue < int > dane2;

    if (punkty == -1)
    {
        clear( dane1 );
        cout << "aaaa";
        clear( dane2 );
        punkty = 0;
    }

    if (ile==punkty)                                                               // jesli kolejka pelna - usuniecie
    {
        t[dane1.front()][dane2.front()] =' ';
        dane1.pop(); dane2.pop();
    }
    dane1.push(x2);
    dane2.push(y2);
    ile=punkty;
}

void clear( queue<int> &q )
{
   queue<int> empty;
   swap( q, empty );
}
void losuj (char t[][31], int x, int y)
    {
        srand(time(NULL));
        do
        {
            celx = rand()%(x-2)+1; cely = (rand()%((y/2)-1))*2+2;
        }while (t[celx][cely]=='x' || t[celx][cely]=='o');
        t[celx][cely]= '*';
    }

int menu(int ilosc_gier)
    {
        int poziom=-1;
        //static bool ilosc_gier;
        do
        {
            char wyb;
            if (!ilosc_gier)
            {
                cout << "Witaj w grze WUNSZ! \n1. Chce grac!\n2. Ranking wynikow \n\n0. Wyjscie\n" << endl;
                wyb=getch();
            }
            else if (ilosc_gier)  wyb='1';

            if (wyb=='1')
            {
                system( "cls" );
                cout << "Wybierz poziom trudnosci: \n1. Latwy \n2. Sredni \n3. Trudny \n4. Extremalnie trudny! \n\n0. Cofnij\n\n";
                char wyb;
                do
                {
                    wyb=getch();
                }while(wyb>'4' || wyb<'0');
                if      (wyb=='1') poziom=20;
                else if (wyb=='2') poziom=10;
                else if (wyb=='3') poziom=4;
                else if (wyb=='4') poziom=2;
                else if (wyb=='0') {poziom=-1; system( "cls" );}
            }
            else if (wyb=='2')
            {
                system( "cls" );
                string linia, imie;
                int nr_linii=1, punkty;

                fstream plik;
                plik.open("ranking.txt", ios::in);

                if(plik.good()==false) {cout<<"Nie mozna otworzyc pliku!"; Sleep( 1000 );system( "cls" ); }
                cout << "Najlepsze wyniki:" << endl;

                while (getline(plik, linia))
                {
                    switch (nr_linii)
                    {
                        case 1: imie=linia; break;
                        case 2: punkty=atoi(linia.c_str()); break;
                    }
                nr_linii++;

                }
                cout<<"Pseudonim: "<<imie<< " Punkty: " << punkty << endl;

                while (1)
                {
                    cout << "\n0. Powrot do menu glownego." << endl;
                    wyb=getch();
                    if (wyb=='0') {system( "cls" ); return menu(0);}
                    else {cout << "Nie ma takiej opcji! Jeszcze raz!"; Sleep( 1000 );system( "cls" );}
                }
            }
            else if (wyb=='0') poziom=0;
            else {cout << "Nie ma takiej opcji! Jeszcze raz!"; Sleep( 1000 );system( "cls" );}
            ilosc_gier=0;
       }while(poziom<0);


        return poziom;
    }

int menu2()
    {
       while (1)
       {
       cout  << "1. Chce grac ponownie!\n2. Zapisz swoj wynik.\n\n0. Wyjscie.\n";
       char wyb = getch();
       if (wyb=='1') return menu(1);
       else if (wyb=='2')
       {
            string imie;
            cout << "Aby zapisac wynik podaj swoj pseudonim" << endl;
            cin>> imie;
            fstream plik;
            plik.open("ranking.txt",ios::out);

            plik<<imie<<endl;
            plik<<punkty<<endl;

            plik.close();

            system( "cls" );

            while (1)
            {
            cout << "Zapisano.\n\n0. Powrot do menu glownego." << endl;
            wyb=getch();
            if (wyb=='0') {system( "cls" ); return menu(0);}
            else {cout << "Nie ma takiej opcji! Jeszcze raz!"; Sleep( 1000 );system( "cls" );}
            }
       }
       else if (wyb=='0') return 0;
       else {cout << "Nie ma takiej opcji! Jeszcze raz!"; Sleep( 1000 );system( "cls" );}
       }
    }


/*void dodaj(char t[][31], int x2, int y2, int punkty)    //  STARA WERSJA dodanie liczb do kolejki
{
    t[x2][y2]='o';

    static int ile=1; dane1[196], dane2[196];

    if (ile>=punkty)                                                               // jesli kolejka pelna - usuniecie
    {
        if (ile == punkty) t[dane1[0]][dane2[0]] = ' ';
        for (int i= 0; i<=punkty; i++)
        {
            dane1[i]=dane1[i+1];    dane2[i]=dane2[i+1];
        }
    }
    dane1[punkty] = x2;    dane2[punkty] = y2;
    ile=punkty;
}*/
