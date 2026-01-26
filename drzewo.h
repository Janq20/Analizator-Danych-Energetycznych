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

    // Metoda pomocnicza dla Iteratora: Spłaszcza całe drzewo do wektora
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

#endif
void dodajDane(int r, int m, int d, shared_ptr<Pomiar> p) {
    if (lata.find(r) == lata.end()) {
        lata[r] = make_shared<Rok>();
    }

    int idx = wyznaczIndeksCwiartki(p->czas);
    auto cwiartka = lata[r]->wezMiesiac(m)->wezDzien(d)->wezCwiartke(idx);

    // --- LOGIKA ANTY-DUPLIKATOWA (zgodnie z wymogiem s. 2 pkt 31) ---
    for (const auto& istniejacy : cwiartka->pomiary) {
        if (istniejacy->czas == p->czas) {
            // Rzucamy wyjÄ…tek, ktĂłry zostanie zalogowany w MenedzerPlikow
            throw runtime_error("Wykryto duplikat - pomijanie rekordu: " + p->czas); 
        }
    }

    cwiartka->dodajPomiar(p);
}
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 9bbcf7b (Implementacja poprawnego iteratora przechodzącego po węzłach drzewa)
class Iterator {
private:
    BazaDanych& baza;
    map<int, shared_ptr<Rok>>::iterator itRok;
    map<int, shared_ptr<Miesiac>>::iterator itMiesiac;
    map<int, shared_ptr<Dzien>>::iterator itDzien;
    map<int, shared_ptr<Cwiartka>>::iterator itCwiartka;
    size_t indexPomiar;

    void ustawNaPoczatek() {
        itRok = baza.lata.begin();
        if (itRok != baza.lata.end()) {
            itMiesiac = itRok->second->miesiace.begin();
            if (itMiesiac != itRok->second->miesiace.end()) {
                itDzien = itMiesiac->second->dni.begin();
                if (itDzien != itMiesiac->second->dni.end()) {
                    itCwiartka = itDzien->second->cwiartki.begin();
                    indexPomiar = 0;
                    pominPuste();
                }
            }
        }
    }

    void pominPuste() {
        while (itRok != baza.lata.end()) {
            if (itMiesiac == itRok->second->miesiace.end()) {
                itRok++;
                if (itRok != baza.lata.end()) itMiesiac = itRok->second->miesiace.begin();
                continue;
            }
            if (itDzien == itMiesiac->second->dni.end()) {
                itMiesiac++;
                if (itMiesiac != itRok->second->miesiace.end()) itDzien = itMiesiac->second->dni.begin();
                continue;
            }
            if (itCwiartka == itDzien->second->cwiartki.end()) {
                itDzien++;
                if (itDzien != itMiesiac->second->dni.end()) itCwiartka = itDzien->second->cwiartki.begin();
                continue;
            }
            if (indexPomiar >= itCwiartka->second->pomiary.size()) {
                itCwiartka++;
                indexPomiar = 0;
                continue;
            }
            break;
        }
    }

public:
    Iterator(BazaDanych& db) : baza(db) {
        ustawNaPoczatek();
    }

    bool czyKoniec() {
        return itRok == baza.lata.end();
    }

    void nastepny() {
        if (czyKoniec()) return;
        indexPomiar++;
        pominPuste();
    }

    shared_ptr<Pomiar> obecny() {
        if (czyKoniec()) return nullptr;
        return itCwiartka->second->pomiary[indexPomiar];
    }
<<<<<<< HEAD
};
>>>>>>> 9bbcf7b (Implementacja poprawnego iteratora przechodzącego po węzłach drzewa)
=======
};
>>>>>>> 9bbcf7b (Implementacja poprawnego iteratora przechodzącego po węzłach drzewa)
