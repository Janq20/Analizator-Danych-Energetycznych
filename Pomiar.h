#ifndef POMIAR_H
#define POMIAR_H

#include <string>
#include <iostream>

using namespace std;

struct Pomiar {
    string czas;        
    float autoKonsumpcja;
    float eksportEnergii;
    float importEnergii;
    float pobor;
    float produkcja;
    Pomiar(string t, float a, float e, float i, float c, float p)
        : czas(t), autoKonsumpcja(a), eksportEnergii(e), importEnergii(i), pobor(c), produkcja(p) {
    }

    void wypisz() const {
        cout << "   [Pomiar] Czas: " << czas
            << " | Import: " << importEnergii
            << " | Pobor: " << pobor << endl;
    }
};

#endif#pragma once
