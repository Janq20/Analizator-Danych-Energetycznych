#ifndef DRZEWO_H
#define DRZEWO_H

#include <map>
#include <vector>
#include <memory>
#include "Pomiar.h"

using namespace std;

// --- Dzieñ: Lista pomiarów ---
class Dzien {
public:
    vector<shared_ptr<Pomiar>> pomiary;

    void dodajPomiar(shared_ptr<Pomiar> p) {
        pomiary.push_back(p);
    }
};

// --- Miesi¹c:  Dni ---
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

// --- Rok: Miesi¹ce ---
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

// --- Baza Danych: Lata ---
class BazaDanych {
public:
    map<int, shared_ptr<Rok>> lata;

    void dodajDane(int r, int m, int d, shared_ptr<Pomiar> p) {
        if (lata.find(r) == lata.end()) {
            lata[r] = make_shared<Rok>();
        }
        lata[r]->wezMiesiac(m)->wezDzien(d)->dodajPomiar(p);
    }
};

#endif