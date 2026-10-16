// ============================================================
//  Lab: Interfejsy i wzorzec Strategy
//  Paradygmaty Programowania Obiektowego
//
// ============================================================

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// ============================================================
//  KLASY PODSTAWOWE
//  Mozesz skopiować tutaj swoje klasy Student i ListaObecnosci,
//  z poprzednich zajec, lub użyć zaproponowanych (uproszczone)
// ============================================================


// ============================================================
// STUDENT
// Bazowa implementacja - tylko podstawowe informacje o studencie
// ustawianie danych przez konstruktor, bez setterów
// ============================================================

class Student {
  private:
    string imie;
    string nazwisko;
    string nrIndeksu;

  public:
    Student(string imie, string nazwisko, string nrIndeksu)
        : imie(imie), nazwisko(nazwisko), nrIndeksu(nrIndeksu) {}

    string getImie()       { return imie; }
    string getNazwisko()   { return nazwisko; }
    string getNrIndeksu() { return nrIndeksu; }
};


// ============================================================
//  ListaObecnosci
//
//  Prosta implementacja oparta o dwie oddzielne tablice polaczone indeksem:
//    studenci[i]  — wskaznik do obiektu Student
//    obecnosc[i]  — licznik obecnosci studenta i
//
//  Indeksy obu tablic sa zsynchronizowane:
//  obecnosc[2] to liczba obecnosci studenta studenci[2].
// ============================================================
class ListaObecnosci {
  private:
    Student** studenci;
    int*      obecnosc;
    int       liczba_zajec;
    int       rozmiar;
    int       pojemnosc;
    float     progZaliczenia;
    string    nazwa_przedmiotu;

  public:
    ListaObecnosci(string nazwa, int pojemnosc)
        : liczba_zajec(0), progZaliczenia(75), rozmiar(0), pojemnosc(pojemnosc),
          nazwa_przedmiotu(nazwa) {
        studenci = new Student*[pojemnosc];
        obecnosc = new int[pojemnosc];
        for (int i = 0; i < pojemnosc; i++)
            obecnosc[i] = 0;
    }

    ~ListaObecnosci() {
        for (int i = 0; i < rozmiar; i++)
            delete studenci[i];
        delete[] studenci;
        delete[] obecnosc;
    }

    void dodajStudenta(Student* s) {
        if (rozmiar < pojemnosc)
            studenci[rozmiar++] = s;
    }

    // Zaznacza obecnosc dla jednych zajec.
    // obecni[i] == 1 oznacza ze student studenci[i] byl obecny.
    // obecni[i] == 0 oznacza nieobecnosc.
    void zaznaczObecnosc(int* obecni, int rozmiar_tablicy) {
        liczba_zajec++;
        for (int i = 0; i < rozmiar_tablicy && i < rozmiar; i++)
            if (obecni[i])
                obecnosc[i]++;
    }

    int getLiczbaObecnosci(int i) { return obecnosc[i]; }
    int getLiczbaZajec()          { return liczba_zajec; }

    int getProcentObecnosci(int i) {
        if (liczba_zajec == 0) return 0;
        return (obecnosc[i] * 100) / liczba_zajec;
    }

    Student**   getStudenci() { return studenci; }
    int*        getObecnosc() { return obecnosc; }
    int         getRozmiar()  { return rozmiar; }
    string getNazwa()    { return nazwa_przedmiotu; }
};


// ============================================================
//  INTERFEJSY
//  Nie modyfikuj tych klas.
// ============================================================

class IFiltr {
  public:
    // Zwraca nowa tablice wskaznikow do studentow spelniajacych
    // kryterium. Rozmiar wynikowej tablicy zapisywany jest
    // do *wynik_rozmiar. Wywolujacy jest odpowiedzialny za
    // zwolnienie pamieci tablicy wynikowej (nie obiektow Student).
    virtual Student** filtruj(Student** studenci,
                              int*      obecnosc,
                              int       liczba_zajec,
                              int       rozmiar,
                              int*      wynik_rozmiar) = 0;
};

class INotyfikator {
  public:
    virtual void powiadom(Student* student, string komunikat) = 0;
};

class IRaport {
  public:
    virtual void generuj(Student** studenci,
                         int*      obecnosc,
                         int       liczba_zajec,
                         int       rozmiar,
                         string nazwa_pliku) = 0;
};


// ============================================================
//  CZESC 1 — IMPLEMENTACJE IFiltr
//
//  Zaimplementuj jedna z ponizszych klas.
//
//  FiltrProguObecnosci — zwraca studentow ktorych procent
//    obecnosci jest nizszy niz prog podany w konstruktorze
//
//  FiltrNazwiska — zwraca studentow ktorych nazwisko
//    zaczyna sie na litere podana w konstruktorze
//
//  FiltrIndeksu — zwraca studentow ktorych numer indeksu
//    zaczyna sie od ciagu znakow podanego w konstruktorze
// ============================================================


//  PRZYKLAD IMPLEMENTACJI TYPU "STUB"
//  FiltrStub zwraca cala tablice, ktora otrzymal na wejsciu
//

class FiltrStub : public IFiltr {
  public:
    Student** filtruj(Student** studenci, int* obecnosc,
                      int liczba_zajec, int rozmiar,
                      int* wynik_rozmiar) {
        Student** wynik = new Student*[rozmiar];
        for (int i = 0; i < rozmiar; i++)
            wynik[i] = studenci[i];
        *wynik_rozmiar = rozmiar;
        return wynik;
    }

};


// TUTAJ ZAIMPLEMENTUJ FILTRY

class FiltrProguObecnosci : public IFiltr {
    float prog_obecnosci;
  public:
    FiltrProguObecnosci(int prog) : prog_obecnosci((1.0f*prog)/100) {}
    Student** filtruj(Student** studenci, int* obecnosc,
                      int liczba_zajec, int rozmiar,
                      int* wynik_rozmiar) {
        Student** wynik = new Student*[rozmiar];
        float procent_obecnosci;
        int licznik = 0;
        for (int i = 0; i < rozmiar; i++) {
            procent_obecnosci = (1.0f*obecnosc[i])/liczba_zajec;
            if(procent_obecnosci >= prog_obecnosci) {
                wynik[licznik] = studenci[i];
                licznik++;
            }
        }
        *wynik_rozmiar = licznik;
        return wynik;
    }
};

class FiltrNazwiska : public IFiltr {
    char litera;
  public:
    FiltrNazwiska(char lit) : litera(lit) {}
    Student** filtruj(Student** studenci, int* obecnosc,
                      int liczba_zajec, int rozmiar,
                      int* wynik_rozmiar) {
        Student** wynik = new Student*[rozmiar];
        int licznik = 0;
        for(int i = 0; i < rozmiar; i++) {
            string nazwisko = studenci[i]->getNazwisko();
            if(nazwisko[0] == litera) {
                wynik[licznik] = studenci[i];
                licznik ++;
            }
        }
        *wynik_rozmiar = licznik;
        return wynik;
    }
};

class FiltrIndeksu : public IFiltr {
    string szukaj_index;
  public:
    FiltrIndeksu(string inx) : szukaj_index(inx) {}
    Student** filtruj(Student** studenci, int* obecnosc,
                      int liczba_zajec, int rozmiar,
                      int* wynik_rozmiar) {
        Student** wynik = new Student*[rozmiar];
        int licznik = 0;
        for(int i = 0; i < rozmiar; i++) {
            string index = studenci[i]->getNrIndeksu();
            string skrocony_index = index.substr(0, szukaj_index.length());
            if(szukaj_index == skrocony_index) {
                wynik[licznik] = studenci[i];
                licznik ++;
            }
        }
        *wynik_rozmiar = licznik;
        return wynik;
    }
};

// ============================================================
//  CZESC 2 — IMPLEMENTACJE INotyfikator
//
//  Zaimplementuj jedna z ponizszych klas.
//
//  NotyfikatorKonsola — wypisuje imie, nazwisko i komunikat
//    na cout
//
//  NotyfikatorPlik — dopisuje wpis do pliku powiadomienia.txt
//    w formacie: [imie nazwisko]: komunikat
//
//  NotyfikatorEmail — wypisuje na konsole sformatowany
//    naglowek emaila:
//      Do: nrIndeksu@uczelnia.pl
//      Temat: Powiadomienie
//      Tresc: komunikat
// ============================================================

class NotyfikatorStub : public INotyfikator {
public:
    void powiadom(Student* student, string komunikat) {
        cout << "[STUB] " << student->getImie() << ": " << komunikat << endl;
    }
};

// TUTAJ ZAIMPLEMENTUJ

class NotyfikatorKonsola : public INotyfikator {
public:
    void powiadom(Student* student, string komunikat){
        cout<<"Imie i nazwisko: "<<student->getImie()<<" "<<student->getNazwisko()<<" "<<komunikat<<endl;
    } 
};

class NotyfikatorPlik : public INotyfikator {
    void powiadom(Student* student, string komunikat) {
        ofstream plik("powiadomienia.txt");

        if (!plik.is_open()) cout << "Nie udalo sie otworzyc pliku.";

        plik << "Imie i nazwisko: " << student->getImie() << " " << student->getNazwisko() << " " << komunikat << endl;

        plik.close();
    }
};

class NotyfikatorEmail : public IFiltr {
    void powiadom(Student* student, string komunikat) {
        cout << "Do: " << student->getNrIndeksu() << " @uczelnia.pl" << endl;
        cout << "Temat: " << "Powiadomienie" << endl;
        cout << "Tresc: " << komunikat << endl;
    }
};

// ============================================================
//  CZESC 3 — IMPLEMENTACJE IRaport
//
//  Zaimplementuj jedna z ponizszych klas (do wyboru).
//
//  RaportFrekwencji — dla kazdego studenta wypisuje imie,
//    nazwisko i procent obecnosci
//
//  RaportZagrozonych — wypisuje tylko studentow ponizej
//    50% obecnosci z informacja ile zajec musza nadrobi
//    zeby osiagnac prog
//
//  RaportPodsumowania — jedna strona: liczba studentow,
//    srednia obecnosc, najlepszy i najgorszy wynik
// ============================================================

// TUTAJ ZAIMPLEMENTUJ
class RaportStub : public IRaport {
public:
    void generuj(Student** studenci, int* obecnosc,
                 int liczba_zajec, int rozmiar, string nazwa_pliku) {
        cout << "[STUB] Raport dla " << rozmiar << " studentow" << endl;
    }
};

class RaportFrekwencji : public IRaport {
    void generuj(Student** studenci, int* obecnosc,
                 int liczba_zajec, int rozmiar, string nazwa_pliku) {
        cout << "Raport dla " << rozmiar << " studentow" << endl;
        float procent_obecnosci = 0;
        for (int i = 0; i < rozmiar; i++) {
            procent_obecnosci = ( (1.0f*obecnosc[i]) / liczba_zajec ) * 100;
            cout << studenci[i]->getImie() << " " << studenci[i]->getNazwisko() << " " << procent_obecnosci << "%" << endl;
        }
    }
};

class RaportZagrozonych : public IRaport {
    void generuj(Student** studenci, int* obecnosc,
                 int liczba_zajec, int rozmiar, string nazwa_pliku) {
        cout << "Raport dla " << rozmiar << " studentow" << endl;
        float procent_obecnosci = 0;
        for (int i = 0; i < rozmiar; i++) {
            procent_obecnosci = ( (1.0f*obecnosc[i]) / liczba_zajec ) * 100;
            if(procent_obecnosci < 50) {
                cout << studenci[i]->getImie() << " " << studenci[i]->getNazwisko() << " " << procent_obecnosci << "%";
                int licznik = 1;
                do {
                    procent_obecnosci = (  ( 1.0f * ( obecnosc[i] + licznik ) ) / ( liczba_zajec + licznik )  ) * 100;
                    licznik++;
                } while(procent_obecnosci < 50);
                cout << " -> Do odrobienia " << licznik << " zajec" << endl;
            }
        }
    }
};

class RaportPodsumowania : public IRaport {
    void generuj(Student** studenci, int* obecnosc,
                 int liczba_zajec, int rozmiar, string nazwa_pliku) {
        cout << "Raport dla " << rozmiar << " studentow" << endl;
        float procent_obecnosci = 0;
        for (int i = 0; i < rozmiar; i++) {
            procent_obecnosci = ( (1.0f*obecnosc[i]) / liczba_zajec ) * 100;
            cout << studenci[i]->getImie() << " " << studenci[i]->getNazwisko() << " " << procent_obecnosci << "%" << endl;
        }
    }
};

// ============================================================
//  MAIN i przyklad filtra
// ============================================================


void testFiltrowania(IFiltr *filtr, ListaObecnosci *lista){
    int wynik_rozmiar_stub = 0;
    Student **wynikStub = filtr->filtruj(lista->getStudenci(),
                                            lista->getObecnosc(),
                                            lista->getLiczbaZajec(),
                                            lista->getRozmiar(),
                                            &wynik_rozmiar_stub);

    for(int i=0; i<wynik_rozmiar_stub; i++){
      cout << wynikStub[i]->getImie() << endl;
    }

    delete[] wynikStub;


}

void testNotyfikatora(INotyfikator* notyfikator, ListaObecnosci* lista) {
    // do uzupelnienia przez studentow
    notyfikator->powiadom(lista->getStudenci()[0],"test");
}

void testRaportu(IRaport* raport, ListaObecnosci* lista) {
    raport->generuj(lista->getStudenci(), lista->getObecnosc(), lista->getLiczbaZajec(), lista->getRozmiar(), "cos.txt");
}

int main() {
    ListaObecnosci lista("Paradygmaty Programowania Obiektowego", 10);

    lista.dodajStudenta(new Student("Anna",    "Kowalska",   "123456"));
    lista.dodajStudenta(new Student("Bartosz", "Nowak",      "654321"));
    lista.dodajStudenta(new Student("Celina",  "Kwiatkowska","111111"));
    lista.dodajStudenta(new Student("Damian",  "Krawczyk",   "222222"));

    // symulacja trzech zajec
    // zajecia 1: Anna, Celina i Damian obecni
    int zajecia1[] = {1, 0, 1, 1};
    lista.zaznaczObecnosc(zajecia1, lista.getRozmiar());

    // zajecia 2: tylko Damian obecny
    int zajecia2[] = {0, 0, 0, 1};
    lista.zaznaczObecnosc(zajecia2, lista.getRozmiar());

    // zajecia 3: Anna i Celina obecne
    int zajecia3[] = {1, 0, 1, 0};
    lista.zaznaczObecnosc(zajecia3, lista.getRozmiar());

    // PRZYKLADY UZYCIA FILTRA, RAPORTU i NOTYFIKATORA na podstawie interfejsów
    FiltrStub filtrStub;
    testFiltrowania(&filtrStub, &lista);

    RaportStub raportStub;
    testRaportu(&raportStub, &lista);

    RaportFrekwencji raportFrekw;
    testRaportu(&raportFrekw, &lista);

    RaportZagrozonych raportZagr;
    testRaportu(&raportZagr, &lista);

    NotyfikatorStub notyfikatorStub;
    testNotyfikatora(&notyfikatorStub, &lista);

    // --- Test IFiltr ---
    // WYKORZYSTAJ FILTR ZAIMPLEMENTOWANY PRZEZ INNA GRUPE

    // --- Test INotyfikator ---
    // WYKORZYSTAJ NOTYFIKATOR ZAIMPLEMENTOWANY PRZEZ INNA GRUPE

    // --- Test IRaport ---
    // WYKORZYSTAJ RAPORT ZAIMPLEMENTOWANY PRZEZ INNA GRUPE

    return 0;
}