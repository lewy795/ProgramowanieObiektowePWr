#include <iostream>
#include <fstream>

using namespace std;

//========= Klasa abstrakcyjna =========

class ISerializable {
public:
    virtual string serialize() = 0;
};

//========= Klasy osob ===========

class Osoba : public ISerializable {
protected:
    int nr_indeksu;
    string imie;
    string nazwisko;

public:
    Osoba(int inx = 0, string im = "", string naz = "") : nr_indeksu(inx), imie(im), nazwisko(naz) {};

    virtual ~Osoba() {}

    void setIndeks(int nowy_indeks) { nr_indeksu = nowy_indeks; }
    void setImie (string nowe_imie) { imie = nowe_imie; }
    void setNazwisko (string nowe_nazwisko) { nazwisko = nowe_nazwisko; }

    virtual void drukuj() { cout << "OSOBA  " << imie << " " << nazwisko << endl; }

    int getIndeks() { return nr_indeksu; }
    string getImie() { return imie; }
    string getNazwisko() { return nazwisko; }

    virtual string serialize() override {
        return "Osoba, imie: " + imie + ", nazwisko: " + nazwisko + ", numer indeksu: " + to_string(nr_indeksu);
    }
};

class Student : public Osoba {
protected:
    string kierunek;
    int semestr;
public:
    Student(int inx = 0, string im = "", string naz = "", string kier = "", int sem = 0) : Osoba(inx, im, naz), kierunek(kier), semestr(sem) {}

    virtual ~Student () {}

    void setKierunek(string nowy_kierunek) { kierunek = nowy_kierunek; }
    void setSemestr(int nowy_sem) { semestr = nowy_sem; }
    
    virtual void drukuj() { cout << "STUDENT  " << imie << " " << nazwisko << " " << kierunek << " " << semestr << endl; }

    string getKierunek() { return kierunek; }
    int getSemestr() {return semestr; }

    virtual string serialize() override {
        return "Student, imie: " + imie + ", nazwisko: " + nazwisko + ", numer indeksu: " + to_string(nr_indeksu) + ", kierunek: " + kierunek + ", semestr " + to_string(semestr);
    }
};

class StudentZaoczny : public Student {
public:
    void drukuj() { cout << "STUDENT ZAOCZNY " << imie << " " << nazwisko << " " << kierunek << " " << semestr << endl; }

    virtual string serialize() override {
        return "Student zaoczny, imie: " + imie + ", nazwisko: " + nazwisko + ", numer indeksu: " + to_string(nr_indeksu) + ", kierunek: " + kierunek + ", semestr " + to_string(semestr);
    }
};

class Prowadzacy : public Osoba {
protected:
    string tytul_naukowy;
    string przedmiot;
public:
    void setTytul(string nowy_tytul) { tytul_naukowy = nowy_tytul; }
    void setPrzedmiot(string nowy_przedmiot) { przedmiot = nowy_przedmiot; }

    void drukuj() { cout << "PROWADZACY " << tytul_naukowy << " " << imie << " " << nazwisko << " " << przedmiot << endl; }

    string getTytul() { return tytul_naukowy; }
    string getPrzedmiot() { return przedmiot; }

    virtual string serialize() override {
        return "Prowadzacy, " + tytul_naukowy + " " + imie + " " + nazwisko + ", numer indeksu: " + to_string(nr_indeksu) + ", przedmiot: " + przedmiot;
    }
};

//========= Klasa listy Obecnosci ============

class ListaObecnosci {
    bool tabObecnosc[10];
    Osoba tabOsob[10];

    int znajdzPusteMiejsce() {
        for (int i = 0; tabOsob[i].getNazwisko() == ""; i++) {
            return i;
        }
        return -1;
    }

    int znajdzOsobe(string nazwisko) {
        for (int i = 0; tabOsob[i].getNazwisko() == nazwisko; i++) {
            return i;
        }
        return -1;
    }

public:
    void dodajOsobe(Osoba nowa_osoba) {
        int i = znajdzPusteMiejsce();
        tabOsob[i] = nowa_osoba;
    }

    void drukujListe() {
        cout << endl << "Nr INDEKSU    IMIE    NAZWISKO   OBECNOSC" << endl;
        for (int i = 0; tabOsob[i].getNazwisko() != ""; i++) {
            cout << tabOsob[i].getIndeks() << "    " << tabOsob[i].getImie() << "    " << tabOsob[i].getNazwisko() << "    " << tabObecnosc[i] << endl;
        }
    }

    void ustawObecnosc(string nazwisko, bool nowa_obecnosc) {
        int i = znajdzOsobe(nazwisko);
        int j = znajdzPusteMiejsce();
        tabOsob[j] = tabOsob[i];
        tabObecnosc[j] = nowa_obecnosc;

        cout << "ustawiam obecnosc \n";
    }
};

//=========== Klasa interfejsu uzytkownika =============

class InterfejsUzytkownika {
    Osoba *tablicaOsob;
    ListaObecnosci *tablicaList;
    int ileOsob, ileList;
    int wybor;

    int znajdzPuste() {
        for (int i = 0; i < ileOsob; i++) {
            if (tablicaOsob[i].getNazwisko().empty()) {
                return i;
            }
        }
        return -1;
    }

    int znajdzNazwisko(string nazwisko) {
        for (int i = 0; i < ileOsob; i++) {
            if (tablicaOsob[i].getNazwisko() == nazwisko) {
                return i;
            }
        }
        return -1;
    }

    void utworzOsobe() {
        int indeks;
        string imie;
        string nazwisko;
        Osoba nowa_osoba;

        cout << "Podaj indeks: ";
        cin >> indeks;
        nowa_osoba.setIndeks(indeks);

        cout << "Podaj imie: ";
        cin >> imie;
        nowa_osoba.setImie(imie);

        cout << "Podaj nazwisko: ";
        cin >> nazwisko;
        nowa_osoba.setNazwisko(nazwisko);

        int i = znajdzPuste();
        tablicaOsob[i] = nowa_osoba;
    }

    void dodajOsobe() {
        string nazwisko;
        int lista;

        cout << "Kogo chcesz dodac (podaj nazwisko): ";
        cin >> nazwisko;

        cout << "Do ktorej listy: ";
        cin >> lista;

        int i = znajdzNazwisko(nazwisko);
        tablicaList[lista].dodajOsobe(tablicaOsob[i]);
    }

    void ustawObecnosc() {
        string nazwisko;
        int lista;
        bool obecnosc;

        cout << "Komu chcesz ustawic obecnosc (podaj nazwisko): ";
        cin >> nazwisko;

        cout << "W ktorej liscie: ";
        cin >> lista;

        cout << "Ustaw obecnosc: ";
        cin >> obecnosc;

        tablicaList[lista].ustawObecnosc(nazwisko, obecnosc);
    }

    void przesun(int index) {
        for (int i = index; i < ileOsob-1; i++) {
            tablicaOsob[i].setIndeks( tablicaOsob[i + 1].getIndeks() );
            tablicaOsob[i].setNazwisko( tablicaOsob[i + 1].getNazwisko() );
            tablicaOsob[i].setImie( tablicaOsob[i + 1].getImie() );
        }
        tablicaOsob[ileOsob-1].setIndeks( 0 );
        tablicaOsob[ileOsob-1].setNazwisko( "" );
        tablicaOsob[ileOsob-1].setImie( "" );
    }

    void usunOsobe() {
        string nazwisko;

        cout << "Kogo chcesz usunac (podaj nazwisko): ";
        cin >> nazwisko;

        int i = znajdzNazwisko(nazwisko);
        przesun (i);
    }

    void edytujOsobe() {
        string nazwisko;
        int nowy_indeks;
        string nowe_nazwisko;
        string nowe_imie;
        Osoba nowa_osoba;

        cout << "Kogo chcesz edytowac (podaj nazwisko): ";
        cin >> nazwisko;
        int i = znajdzNazwisko(nazwisko);

        cout << "Podaj nowy indeks: ";
        cin >> nowy_indeks;
        tablicaOsob[i].setIndeks(nowy_indeks);

        cout << "Podaj nowe imie: ";
        cin >> nowe_imie;
        tablicaOsob[i].setImie(nowe_imie);

        cout << "Podaj nowe nazwisko: ";
        cin >> nowe_nazwisko;
        tablicaOsob[i].setNazwisko(nowe_nazwisko);
    }

    void wyswietlListe() {
        int lista;
        cout << "Ktora liste chcesz wyswietlic: ";
        cin >> lista;

        system("clear");

        tablicaList[lista].drukujListe();
    }

    void zapiszDoPliku(ISerializable *obj) {
        ofstream plik("/Users/ml/Desktop/programming/obiektowe/db.txt");

        if(!plik.is_open()) {
            cout << "Nie udalo sie otworzyc pliku. Sproboj jeszcze raz.";
        }

        plik << obj->serialize() << endl;

        cout << "Udalo sie zapisac do pliku.\n";

        system("clear");

        plik.close();
    }

public:

    InterfejsUzytkownika(Osoba *tbos, int os, ListaObecnosci *tbls, int ls) : tablicaOsob(tbos), ileOsob(os), tablicaList(tbls), ileList(ls) {}

    //============== Petla ===============

    void petla() {
        while(true) {
            cout << "=============================\n";
            cout << "========== M E N U ==========\n \n";

            cout << "1. Utworz osobe\n";
            cout << "2. Dodaj osobe do listy\n";
            cout << "3. Ustaw obecnosc osoby na liscie\n";
            cout << "4. Usun osobe z listy\n";
            cout << "5. Zmien dane osoby\n";
            cout << "6. Wyswietl liste\n";
            cout << "7. Zapisz do pliku\n \n";

            cout << "Wybierz: ";
            cin >> wybor;

            system("clear");

            switch(wybor) {
                case 1:
                    utworzOsobe();
                    system("clear");
                    break;
                case 2:
                    dodajOsobe();
                    system("clear");
                    break;
                case 3:
                    ustawObecnosc();
                    system("clear");
                    break;
                case 4:
                    usunOsobe();
                    system("clear");
                    break;
                case 5:
                    edytujOsobe();
                    system("clear");
                    break;
                case 6:
                    wyswietlListe();
                    break;
                case 7: 
                    system("clear");
                    cout << "Co zapisac?\n";
                    cout << "1. Wszystkie osoby\n";
                    cout << "2. Wybrana osobe\n \n";
                    cin >> wybor;
                    system("clear");
                    switch(wybor) {
                        case 1:
                            for(int i; i < ileOsob; i++) {
                                zapiszDoPliku(&tablicaOsob[i]);
                            }
                            break;
                        case 2: {
                            string nazw;
                            cout << "Podaj nazwisko osoby, ktora chcesz zapisac: ";
                            cin >> nazw;
                            int i = znajdzNazwisko(nazw);
                            zapiszDoPliku(&tablicaOsob[i]);
                            break;
                        }
                        default: 
                            system("clear");
                            cout << endl << "Brak takiej opcji." << endl;
                            break;
                    }
                    break;
                default: 
                    system("clear");
                    cout << endl << "Brak takiej opcji." << endl;
                    break;
            };
        }
    };
};

//============ MAIN ============

int main() {

    Osoba tablicaOsob[10];
    ListaObecnosci tablicaList[2];
    InterfejsUzytkownika ui(tablicaOsob, 10, tablicaList, 2);

    ui.petla();

    return 0;
}
