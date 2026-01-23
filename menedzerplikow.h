#ifndef MENEDZER_PLIKOW_H
#define MENEDZER_PLIKOW_H

#include <iostream>
#include <fstream>
#include <string>
#include "Drzewo.h"
#include "Narzedzia.h"

using namespace std;

class MenedzerPlikow {
private:
    Logger logger;

public:
    void wczytajPlik(const string& nazwaPliku, BazaDanych& db) {
        ifstream plik(nazwaPliku);

        if (!plik.is_open()) {
            cout << "BLAD: Nie mozna otworzyc pliku: " << nazwaPliku << endl;
            logger.logujBlad("Nieudane otwarcie pliku", nazwaPliku);
            return;
        }

        cout << "Plik otwarty pomyslnie: " << nazwaPliku << endl;

        plik.close();
    }
};

#endif