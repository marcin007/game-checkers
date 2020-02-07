#include <iostream>
#include <cstdlib>

using namespace std;


const char czarne_pole = ' ';
const char biale_pole = '.';
const char literki[] = "ABCDEFGHI";
const char sciana ='|';
const char znak_biale = 'o';
const char znak_czarne = 'x';

void RysujPlansze(int&, int*, int*, int []);
void WybieraniePiona(int&, int*, int&);
void RuchPionem(int&, int*, int*, int&, int&, int&);
void PiszLitery(int&);
bool CzyJestPionek(int&, int*, int&);
int SprawdzKordy(char [], int&);
int SprawdzIndeksLitery(char&, int&);
bool CzyWolnaPozycja(int&, int&, int*, int*, int&, int&);
bool CzyMoznaZrobicRuch(int&, int&, int&, int*, int*, int&, int&);

int main()
{
	int pozycja_czarnego_piona;
	int pozycja_bialego_piona;
	bool ruch_bialych = true;
	bool koniec_gry = false;
	int x;
	cout << "podaj wielkosc planszy 3<x<10" <<endl;
	cin >> x;
	int ilosc_pionow[2], ilosc_ruchow=0;
	ilosc_pionow[0]=0;
	ilosc_pionow[1]=0;
	int *biale = new int [x];
	int *czarne = new int [x];
    for(int i=0; i<x; i++) //ustalanie ze piona nie ma na planszy i nie zostal jeszcze zbity
    {                       //1-nie ma na planszy
        biale[i]=-10;       //0-nie zostal zbity
        czarne[i]=-10;      //czyli 10
    }


	while(koniec_gry==false){
        ilosc_ruchow++;
		system("cls");
		if(ruch_bialych)
        {
            cout << "tura bialych [o]" << endl;
        }else
        {
            cout << "tura czarnych [x]" << endl;
        }
		RysujPlansze(x, biale, czarne, ilosc_pionow);

		if(ruch_bialych) //ruch bialych
        {
            WybieraniePiona(x, biale, pozycja_bialego_piona);
            RuchPionem(x, biale, czarne, pozycja_bialego_piona, ilosc_pionow[1], ilosc_ruchow);
            ruch_bialych=false;
            if(!ilosc_pionow[1])
            {
                koniec_gry=true;
                cout << "wygrywaja biale" << endl;
            }
        }else //ruch czarnych
        {
            WybieraniePiona(x, czarne, pozycja_czarnego_piona);
            RuchPionem(x, czarne, biale, pozycja_czarnego_piona, ilosc_pionow[0], ilosc_ruchow);
            ruch_bialych=true;
            if(!ilosc_pionow[0]) //jezeli przeciwnikowi zostalo 0 pionow to wygrywa gracz
            {                   //!0 == 1, czyli dla 0 warunek jest spelniony
                koniec_gry=true;
                cout << "wygrywaja czarne" << endl;
            }
        }
        if(ilosc_ruchow==20){
            koniec_gry=true;
            cout << "przez 10 ruchow nie bylo bicia, jest remis" << endl;
        }
	}
    delete [] biale; //usuwanie tablicy dynamicznej (zwalnianie pamieci)
    biale = NULL;
    delete [] czarne;
    czarne = NULL;
    return 0;
}

void RysujPlansze(int &x, int *biale, int *czarne, int ilosc_pionow[])
{
    bool  czarny=false;

    for(int  i = x; i > 0; i--){
		cout << sciana; //rysuje scianke
		for(int j = 0 ; j < x; j++){
			int pozycja = (i-1)*x + j; //obliczanie pozycji, na ktorej jest cos rysowane
			if(czarny == true){
			for(int k=0; k<x; k++){
                if(pozycja == biale[k]){ //jezeli pionek istnieje to ma go narysowac
                    cout << znak_biale;
                    break;
                }
                else if(pozycja == czarne[k]){ //jezeli pionek istnieje to ma go narysowac
                    cout << znak_czarne;
                    break;
                }
                else if(biale[k]==-10){ //jezeli pionek nie istnieje to ma nadac mu pozycje
                    cout << znak_biale;
                    biale[k]=pozycja;
                    ilosc_pionow[0]++;
                    break;
                }else if(czarne[k]==-10 && ilosc_pionow[0]==x && i<3) { //jezeli pionek nie istnieje to ma nadac mu pozycje
                    cout << znak_czarne;
                    czarne[k]=pozycja;
                    ilosc_pionow[1]++;
                    break;
                }else if(k==x-1){ //jezeli nie ma pionka to ma byc czarne pole
                    cout << czarne_pole;
                    break;
                }
			}
			}else{
				cout << biale_pole; //rysuje biale pole
			}

		cout << sciana; // rysuje scianke
		bool ostatnie = (j==(x-1));
		if(!ostatnie || x%2==1)czarny = !czarny; //ustala czy pole ma byc czarne czy biale w nastepnym wierszu
	}                                            //inaczej dla x parzysty, inaczej dla x nieparzystych
		cout << i << endl;
	}
	PiszLitery(x); //wypisuje litery pod szachownica
}

void WybieraniePiona(int &x, int* gracz, int &pozycja_piona_gracza)
{
    char znak_pozycji_piona[] = "  ";
    bool nie_wybrano_piona = true;
    while(nie_wybrano_piona){
        cout << "Podaj pozycje piona, ktorego chcesz ruszyc: " << endl;
        cin >> znak_pozycji_piona;
        int pozycja = SprawdzKordy(znak_pozycji_piona, x);                       //sprawdzanie czy na podanej
        if(pozycja != -1 && CzyJestPionek(pozycja, gracz, x)){                     //pozycji znajduje sie pionek
            pozycja_piona_gracza = pozycja;
            nie_wybrano_piona = false;
        }else{
            cout << "Na tej pozycji nie ma piona! \nLub ruch piona jest niemozliwy\n" << endl;
        }
    }
}

void RuchPionem(int& x, int* gracz, int* przeciwnik, int& pozycja_piona_gracza, int& ilosc_pionow, int& ilosc_ruchow)
{
    char znak_nastepnej_pozycji[] = "  ";
    bool mozna_zrobic_ruch = true;
    while(mozna_zrobic_ruch){
        cout << "Podaj nastepna pozycje piona:" << endl;
        cin >> znak_nastepnej_pozycji;
        int nastepna_pozycja = SprawdzKordy(znak_nastepnej_pozycji, x);                                           //oblicza nastepna pozycje
        if(nastepna_pozycja != -1 && CzyMoznaZrobicRuch(pozycja_piona_gracza, nastepna_pozycja, x, gracz, przeciwnik, ilosc_pionow, ilosc_ruchow)){  //sprawdzenie dostepnosci ruchu
            for(int i=0; i<x; i++)
                    if(pozycja_piona_gracza == gracz[i]){
                    gracz[i] = nastepna_pozycja;
                    mozna_zrobic_ruch = false;
                }
        }
        if(mozna_zrobic_ruch){
            cout << "Nieprawidlowy ruch! " << endl;
        }
    }
}

void PiszLitery(int &x)
{
    cout << ' ';
	for(int j=0; j < x; j++){  //rysowanie osi znaków ABCD
			cout << literki[j] << ' ';
		}
	cout << endl;
}

bool CzyJestPionek(int &pos, int* kolor, int &x){ //sprawdza czy jest pionek i ruch nim jest mozliwy
    for(int i=0; i<x; i++)
    if(pos == kolor[i]){
        for(int k=0; k<x; k++){
            if(pos==0 && pos+(x+1) == kolor[k] || pos==(x*x)-1 && pos-(x+1) == kolor[k]) //rogi
                return false;
            for(int j=0; j<x; j++){
                if(pos%x == 0 && pos+(x+1) == kolor[j] && pos-(x-1) == kolor[k]) //lewa sciana
                    return false;
                else if(pos%x == x-1 && pos+(x-1) == kolor[j] && pos-(x+1) == kolor[k]) //prawa sciana
                    return false;
                else if(pos < x-1 && pos+(x-1) == kolor[j] && pos+(x+1) == kolor[k]) //dolna sciana
                    return false;
                else if(pos > (x-1)*x && pos-(x-1) == kolor[j] && pos-(x+1) == kolor[k]) //gorna sciana
                    return false;
                else
                for(int p=0; p<x; p++)
                for(int o=0; o<x; o++)
                if(pos+(x-1) == kolor[j] && pos+(x+1) == kolor[k] && pos-(x-1) == kolor[o] && pos-(x+1) == kolor[p])
                    return false;
            }
        }
        return true;
    }
	return false;
}

int SprawdzKordy(char pos[], int &x){
	int i = pos[1]-48;
	int j = SprawdzIndeksLitery(pos[0], x);
	if(j == -1){
		return -1;
	}                                                                   /*obliczanie pozycji kolejnego ruchu*/
	if(i > x || i < 1){
		return -1;
	}
	int position = (i-1)*x+j;
	return position;
}

int SprawdzIndeksLitery(char &litera, int &x){
	for(int i = 0; i < x; i++){                          //funkcja przyporzadkowuje cyfrom litery z tablicy X
		if(litera == literki[i]){
			return i;
		}
	}
	return -1;
}

bool CzyWolnaPozycja(int &nastepna_pozycja, int &x, int* gracz, int* przeciwnik, int &ilosc_pionow_przeciwnika, int& ruch) {
    for(int i=0; i<x; i++)
	if(nastepna_pozycja == gracz[i]){
		return false;
	}else if(nastepna_pozycja == przeciwnik[i]){ // bicie piona przeciwnika
	    ilosc_pionow_przeciwnika--;
	    przeciwnik[i]=-11;        //1-nie ma piona na planszy, 1-zostal zbity, czyli 11
	}                                                                //funkcja sprawdza czy pole kolejnego ruchu jest wolne
	return true;                                                    //lub na nastepnej pozycji jest pionek przeciwnika
}

bool CzyMoznaZrobicRuch(int &pozycja, int &nastepna_pozycja, int &x, int* gracz, int* przeciwnik,int &ilosc_pionow_przeciwnika, int &ruch) {
	int diff = abs(nastepna_pozycja - pozycja);
	for(int i=1; i<x-1; i++){
        if(diff == (x-1)*i) {
            for(int j=1; j<i; j++)
            {
                for(int k=0; k<x; k++)
                if(gracz[k] == (x-1)*j || przeciwnik[k] == (x-1)*j) // sprawdza czy pionek nie przeskoczy jakiegos piona
                return false;
            }
            return CzyWolnaPozycja(nastepna_pozycja, x, gracz, przeciwnik, ilosc_pionow_przeciwnika, ruch);
        }
        if(diff == (x+1)*i){
            for(int j=1; j<i; j++)
            {
                for(int k=0; k<x; k++)
                if(gracz[k] == (x+1)*j || przeciwnik[k] == (x+1)*j) // tez sprawdza czy niczego pionek nie przeskakuje, tylko dla ruchu po innej linii
                return false;
            }
            return CzyWolnaPozycja(nastepna_pozycja, x, gracz, przeciwnik, ilosc_pionow_przeciwnika, ruch);
        }
	}
	return false;
}