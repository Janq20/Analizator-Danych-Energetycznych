#ifndef DRZEWO_H
#define DRZEWO_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "Pomiar.h"

using namespace std;

// --- Struktura Drzewa ---
class Cwiartka {
public:
    vector<shared_ptr<Pomiar>> pomiary;

    void dodajPomiar(shared_ptr<Pomiar> p) {
        pomiary.push_back(p);
        sort(pomiary.begin(), pomiary.end(), [](const shared_ptr<Pomiar>& a, const shared_ptr<Pomiar>& b) {
            return a->czas < b->czas;
            });
    }
};

class Dzien {
public:
    map<int, shared_ptr<Cwiartka>> cwiartki;

    shared_ptr<Cwiartka> wezCwiartke(int numerCwiartki) {
        if (cwiartki.find(numerCwiartki) == cwiartki.end()) {
            cwiartki[numerCwiartki] = make_shared<Cwiartka>();
        }
        return cwiartki[numerCwiartki];
    }
};

class Miesiac {
public:
    map<int, shared_ptr<Dzien>> dni;

    shared_ptr<Dzien> wezDzien(int numerDnia) {
        if (dni.find(numerDnia) == dni.end()) {
            dni[numerDnia] = make_shared<Dzien>();
        }
        return dni[numerDnia];
    }
};

class Rok {
public:
    map<int, shared_ptr<Miesiac>> miesiace;

    shared_ptr<Miesiac> wezMiesiac(int numerMiesiaca) {
        if (miesiace.find(numerMiesiaca) == miesiace.end()) {
            miesiace[numerMiesiaca] = make_shared<Miesiac>();
        }
        return miesiace[numerMiesiaca];
    }
};

class BazaDanych {
public:
    map<int, shared_ptr<Rok>> lata;

    int wyznaczIndeksCwiartki(string czas) {
        size_t spacja = czas.find(' ');
        if (spacja == string::npos) return 0;

        string godzinaStr = czas.substr(spacja + 1);
        int godzina = stoi(godzinaStr.substr(0, godzinaStr.find(':')));

        if (godzina < 6) return 0;
        if (godzina < 12) return 1;
        if (godzina < 18) return 2;
        return 3;
    }

    void dodajDane(int r, int m, int d, shared_ptr<Pomiar> p) {
        if (lata.find(r) == lata.end()) {
            lata[r] = make_shared<Rok>();
        }

        int idx = wyznaczIndeksCwiartki(p->czas);
        lata[r]->wezMiesiac(m)->wezDzien(d)->wezCwiartke(idx)->dodajPomiar(p);
    }

    // Metoda pomocnicza dla Iteratora: Sp³aszcza ca³e drzewo do wektora
    vector<shared_ptr<Pomiar>> pobierzWszystkiePomiary() {
        vector<shared_ptr<Pomiar>> wynik;
        for (auto const& [r, objRok] : lata) {
            for (auto const& [m, objMiesiac] : objRok->miesiace) {
                for (auto const& [d, objDzien] : objMiesiac->dni) {
                    for (auto const& [c, objCwiartka] : objDzien->cwiartki) {
                        wynik.insert(wynik.end(), objCwiartka->pomiary.begin(), objCwiartka->pomiary.end());
                    }
                }
            }
        }
        return wynik;
    }

    void wypiszStrukture() {
    }
};

// --- Nowoœæ: Klasa Iterator ---
class Iterator {
private:
    vector<shared_ptr<Pomiar>> dane;
    size_t index;

public:
    // Konstruktor: pobiera dane z bazy i ustawia siê na pocz¹tku
    Iterator(BazaDanych& db) {
        dane = db.pobierzWszystkiePomiary();
        index = 0;
    }
    bool czyKoniec() {
        return index >= dane.size();
    }

    // Zwraca wskaŸnik na obecny pomiar
    shared_ptr<Pomiar> obecny() {
        if (czyKoniec()) return nullptr;
        return dane[index];
    }

    // Przesuwa iterator o jeden do przodu
    void nastepny() {
        index++;
    }

    // Resetuje iterator na pocz¹tek
    void naPoczatek() {
        index = 0;
    }
};

#endif