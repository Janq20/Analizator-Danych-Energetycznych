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

    Pomiar(string t = "", float a = 0, float e = 0, float i = 0, float c = 0, float p = 0)
        : czas(t), autoKonsumpcja(a), eksportEnergii(e), importEnergii(i), pobor(c), produkcja(p) {}
};

#endif