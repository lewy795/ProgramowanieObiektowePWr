#include <iostream>

using namespace std;

//========= Klasy osob ===========

class Osoba {
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
};

class StudentZaoczny : public Student {
public:
    void drukuj() { cout << "STUDENT ZAOCZNY " << imie << " " << nazwisko << " " << kierunek << " " << semestr << endl; }
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
};

int main() {
    Osoba *tab[4];
    Osoba os1;
    Student st1;
    Prowadzacy pr1;
    StudentZaoczny stza1;

    tab[0] = &os1;
    tab[1] = &st1;
    tab[2] = &pr1;
    tab[3] = &stza1;

    cout << endl;

    tab[0] -> drukuj();
    tab[1] -> drukuj();
    tab[2] -> drukuj();
    tab[3] -> drukuj();

    cout << endl;

    os1.drukuj();
    st1.drukuj();
    pr1.drukuj();
    stza1.drukuj();

    return 0;
}