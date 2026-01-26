#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <iostream>
#include <vector>
#include <functional>
#include <cmath> 
#include "Drzewo.h"
#include "Pomiar.h"

using namespace std;

class Analizator {
private:
    BazaDanych& db;

public:
    Analizator(BazaDanych& baza) : db(baza) {}
    float obliczSume(function<float(shared_ptr<Pomiar>)> selector) {
        Iterator it(db);
        float suma = 0.0f;
        while (!it.czyKoniec()) {
            auto p = it.obecny();
            if (p) suma += selector(p);
            it.nastepny();
        }
        return suma;
    }
    float obliczSrednia(function<float(shared_ptr<Pomiar>)> selector) {
        Iterator it(db);
        float suma = 0.0f;
        int licznik = 0;

        while (!it.czyKoniec()) {
            auto p = it.obecny();
            if (p) {
                suma += selector(p);
                licznik++;
            }
            it.nastepny();
        }
        return (licznik > 0) ? (suma / licznik) : 0.0f;
    }

    void wyszukajWartosc(float szukana, float tolerancja, string nazwaPola, function<float(shared_ptr<Pomiar>)> selector) {
        Iterator it(db);
        cout << "\n[Szukanie] " << nazwaPola << " w okolicach " << szukana << " (+/- " << tolerancja << "):" << endl;

        bool znaleziono = false;
        while (!it.czyKoniec()) {
            auto p = it.obecny();
            if (p) {
                float wartosc = selector(p);
                if (abs(wartosc - szukana) <= tolerancja) {
                    cout << "  -> Znaleziono: " << p->czas << " | Wartosc: " << wartosc << endl;
                    znaleziono = true;
                }
            }
            it.nastepny();
        }
        if (!znaleziono) cout << "  -> Brak wynikow spelniajacych kryteria." << endl;
    }

    void wypiszStatystyki() {
        cout << "\n==========================================" << endl;
        cout << "        RAPORT ANALITYCZNY" << endl;
        cout << "==========================================" << endl;
        auto getAuto = [](auto p) { return p->autoKonsumpcja; };
        auto getExp = [](auto p) { return p->eksportEnergii; };
        auto getImp = [](auto p) { return p->importEnergii; };
        auto getPob = [](auto p) { return p->pobor; };
        auto getProd = [](auto p) { return p->produkcja; };

        // Tabela wynik�w
        cout << "Kategoria       | Suma [W]       | Srednia [W]" << endl;
        cout << "----------------------------------------------" << endl;
        cout << "Autokonsumpcja  | " << obliczSume(getAuto) << "\t | " << obliczSrednia(getAuto) << endl;
        cout << "Eksport         | " << obliczSume(getExp) << "\t | " << obliczSrednia(getExp) << endl;
        cout << "Import          | " << obliczSume(getImp) << "\t | " << obliczSrednia(getImp) << endl;
        cout << "Pobor           | " << obliczSume(getPob) << "\t | " << obliczSrednia(getPob) << endl;
        cout << "Produkcja       | " << obliczSume(getProd) << "\t | " << obliczSrednia(getProd) << endl;
        cout << "----------------------------------------------" << endl;
    }
};

#endif
void szukajZWarunkiem(string nazwa, float szukana, float tolerancja, string start, string koniec, function<float(shared_ptr<Pomiar>)> selector) {
    Iterator it(db);
    cout << "\n[FILTR] Szukanie: " << nazwa << " | Cel: " << szukana << " (+/- " << tolerancja << ")" << endl;
    cout << "[ZAKRES] " << start << " - " << koniec << endl;

    bool znaleziono = false;
    while (!it.czyKoniec()) {
        auto p = it.obecny();
        // Sprawdzenie czasu i wartości 
        if (p && p->czas >= start && p->czas <= koniec) {
            float wartosc = selector(p);
            if (abs(wartosc - szukana) <= tolerancja) {
                cout << "  MATCH -> Data: " << p->czas << " | Wartosc: " << wartosc << " W" << endl;
                znaleziono = true;
            }
        }
        it.nastepny();
    }
    if (!znaleziono) cout << "  Brak rekordow spelniajacych kryteria." << endl;
}