#ifndef DRZEWO_H
#define DRZEWO_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm> 
#include "Pomiar.h"

using namespace std;

// --- 1. Nowa klasa: Æwiartka (6 godzin) ---
class Cwiartka {
public:
    vector<shared_ptr<Pomiar>> pomiary;

    void dodajPomiar(shared_ptr<Pomiar> p) {
        pomiary.push_back(p);
    }
};

// --- 2. Aktualizacja: Dzieñ ma teraz Æwiartki, a nie Pomiary ---
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

// --- Miesi¹c (bez zmian) ---
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

// --- Rok (bez zmian) ---
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

// --- Baza Danych ---
class BazaDanych {
public:
    map<int, shared_ptr<Rok>> lata;

    void dodajDane(int r, int m, int d, shared_ptr<Pomiar> p) {
        if (lata.find(r) == lata.end()) {
            lata[r] = make_shared<Rok>();
        }
        lata[r]->wezMiesiac(m)->wezDzien(d)->wezCwiartke(0)->dodajPomiar(p);
    }
    void wypiszStrukture() {
    }
};

#endif