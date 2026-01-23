#ifndef ANALIZATOR_H
#define ANALIZATOR_H

#include <iostream>
#include <vector>
#include <functional> 
#include "Drzewo.h"
#include "Pomiar.h"

using namespace std;

class Analizator {
private:
    BazaDanych& db; 

public:
    Analizator(BazaDanych& baza) : db(baza) {}

    // Metoda ogólna do sumowania dowolnego pola (korzysta z lambdy)
    float obliczSume(function<float(shared_ptr<Pomiar>)> selector) {
        Iterator it(db); 
        float suma = 0.0f;

        while (!it.czyKoniec()) {
            auto p = it.obecny();
            if (p) {
                suma += selector(p);
            }
            it.nastepny();
        }
        return suma;
    }

    void wypiszStatystyki() {
        cout << "\n=== WYNIKI ANALIZY ===" << endl;

        // Lambda: p -> p->autoKonsumpcja
        float sumaAuto = obliczSume([](shared_ptr<Pomiar> p) { return p->autoKonsumpcja; });
        cout << "Calkowita Autokonsumpcja: " << sumaAuto << " W" << endl;

        // Lambda: p -> p->eksportEnergii
        float sumaEksport = obliczSume([](shared_ptr<Pomiar> p) { return p->eksportEnergii; });
        cout << "Calkowity Eksport:        " << sumaEksport << " W" << endl;

        // Lambda: p -> p->importEnergii
        float sumaImport = obliczSume([](shared_ptr<Pomiar> p) { return p->importEnergii; });
        cout << "Calkowity Import:         " << sumaImport << " W" << endl;

        // Lambda: p -> p->pobor
        float sumaPobor = obliczSume([](shared_ptr<Pomiar> p) { return p->pobor; });
        cout << "Calkowity Pobor:          " << sumaPobor << " W" << endl;

        // Lambda: p -> p->produkcja
        float sumaProdukcja = obliczSume([](shared_ptr<Pomiar> p) { return p->produkcja; });
        cout << "Calkowita Produkcja:      " << sumaProdukcja << " W" << endl;
    }
};

#endif