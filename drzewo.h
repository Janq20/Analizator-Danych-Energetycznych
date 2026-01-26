#ifndef DRZEWO_H
#define DRZEWO_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "Pomiar.h"

using namespace std;

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
            // Rzucamy wyjątek, który zostanie zalogowany w MenedzerPlikow
            throw runtime_error("Wykryto duplikat - pomijanie rekordu: " + p->czas); 
        }
    }

    cwiartka->dodajPomiar(p);
}