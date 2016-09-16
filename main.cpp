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
void losuj (char t[][31], int x, int y);
void wypisz(int x, int y, char t[][31]);
int wybierzOpcje(int ile);
int wypiszMenu(int co);
int wyswietlWyniki();
int zapiszWynik(int gdzieZapisac);
int menu (int gdzieZapisac);
int celx, cely, punkty, poGrze=0;

int main()
{
    int predkosc, x=15, y=31;

    while(predkosc=menu(predkosc))
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
    system("cls");
    poGrze=1;
    }
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
        while (dane1.size()>0)     dane1.pop();
        while (dane2.size()>0)     dane2.pop();
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

void losuj (char t[][31], int x, int y)
    {
        srand(time(NULL));
        do
        {
            celx = rand()%(x-2)+1; cely = (rand()%((y/2)-1))*2+2;
        }while (t[celx][cely]=='x' || t[celx][cely]=='o');

        t[celx][cely]= '*';
    }

int wypiszMenu(int co)
{
    if (co!=3 && co!=2) system( "cls" );
    if (co==0)
    {
        cout << "Witaj w grze WUNSZ! \n"
            "1. Chce grac!\n"
            "2. Ranking wynikow \n"
            "\n"
            "0. Wyjscie\n" << endl;
        return 2;
    }
    else if (co==1)
    {
        cout << "Wybierz poziom trudnosci:\n"
            "1. Latwy \n"
            "2. Sredni \n"
            "3. Trudny \n"
            "4. Ekstremalnie trudny! \n"
            "\n"
            "0. Cofnij\n" << endl;
        return 4;
    }
    else if (co==2)
    {
        cout << "1. Chce grac ponownie!\n"
            "2. Zapisz swoj wynik.\n"
            "\n"
            "0. Wyjscie\n" << endl;
        return 2;
    }
    else if (co==3)
    {
        cout << "\n"
                "0. Cofnij\n" << endl;
        return 0;
    }

}
int wybierzOpcje(int ile)
{
    char wyb;
    do
    {
        wyb=getch();
        if(wyb<48 || wyb>(48+ile))
        {
            cout << "Nie ma takiej opcji! Jeszcze raz!";
            Sleep( 1000 );
            cout << "\r                                         \r";
        }
    }while(wyb<48 || wyb>(48+ile));
    return ((int)wyb)-48;
}


int menu(int gdzieZapisac)
{
    int poziom=-1;
    do
    {
        int wyb;
        if (!poGrze)
        {
            wyb = wybierzOpcje(wypiszMenu(0));
        }
        else if (poGrze)
        {
            cout << "Przegrales. Liczba zdobytych punktow: " << punkty << endl;
            wyb = wybierzOpcje(wypiszMenu(2));
            if (wyb==2)
                {zapiszWynik(gdzieZapisac); wyb = wybierzOpcje(wypiszMenu(0));}
        }

        if (wyb==1)
        {
            wyb = wybierzOpcje(wypiszMenu(1));
            if      (wyb==1) poziom=20;
            else if (wyb==2) poziom=10;
            else if (wyb==3) poziom=4;
            else if (wyb==4) poziom=2;
            else if (wyb==0) {poziom=-1; system( "cls" );}
        }
        else if (wyb==2)
        {
            wyswietlWyniki();
        }
        else if (wyb==0) poziom=0;

        poGrze=0;
    }while(poziom<0);

return poziom;
}

int wyswietlWyniki()
{
    int wyb;
    system( "cls" );
    string linia, imie, tekst;
    int nr_linii=0, punkty;

    fstream plik;

    cout << "Najlepsze wyniki:" << endl;
    for (int i=0; i<4; i++)
    {
        if      (i==0) {plik.open("rankingLatwy.txt",    ios::in); tekst = "Latwy";}
        else if (i==1) {plik.open("rankingSredni.txt",   ios::in); tekst = "Sredni";}
        else if (i==2) {plik.open("rankingTrudny.txt",   ios::in); tekst = "Trundy";}
        else if (i==3) {plik.open("rankingExtreme.txt",  ios::in); tekst = "Ekstremalnie trudny";}


    if(plik.good()==false) {cout<<"Nie mozna otworzyc pliku: ranking"<<(tekst=="Ekstremalnie trudny" ? "Extreme" : tekst)<<".txt."<<endl; Sleep( 1000 ); }
    else
    {

        cout << "\n\t" << tekst<< endl;
        cout<<"Pseudonim: \t\tPunkty: " << endl;
        while(getline(plik, linia))
        {

            switch (nr_linii%2)
            {
                case 0: imie=linia;                 cout<<imie<<" \t\t\t"; break;
                case 1: punkty=atoi(linia.c_str()); cout<< punkty << endl; break;
            }
            nr_linii++;
        }
    }
    plik.close();
    }
    cout << endl;
    return wybierzOpcje(wypiszMenu(3));
}

int zapiszWynik(int gdzieZapisac)
{
    int wyb;
    string imie;

    cout << "Aby zapisac wynik podaj swoj pseudonim" << endl;
    cin>> imie;

    fstream plik;
    switch (gdzieZapisac)
    {
        case 20: plik.open("rankingLatwy.txt",ios::app ); break;
        case 10: plik.open("rankingSredni.txt",ios::app ); break;
        case 4:  plik.open("rankingTrudny.txt",ios::app ); break;
        case 2:  plik.open("rankingExtreme.txt",ios::app ); break;
    }

    plik<<imie<<endl;
    plik<<punkty<<endl;

    plik.close();

    system( "cls" );
    cout << "Zapisano." <<endl;
    return wybierzOpcje(wypiszMenu(3));
}

/*void dodaj(char t[][31], int x2, int y2)    //  STARA WERSJA dodanie liczb do kolejki
{
    t[x2][y2]='o';
    if (punkty == -1)
    {
        punkty = 0;
    }
    static int ile=1, dane1[196], dane2[196];

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
}
*/
