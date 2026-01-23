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

        string linia;
        int licznikLinii = 0;

        cout << "Rozpoczynam czytanie linii..." << endl;

        while (getline(plik, linia)) {
            licznikLinii++;

            // Wymóg: Pierwsza linia to nag³ówek 
            if (licznikLinii == 1) continue;

            // Zabezpieczenie przed pustymi liniami na koñcu pliku
            if (linia.empty()) continue;
        }

        plik.close();
        cout << "Koniec czytania. Przetworzono linii: " << licznikLinii << endl;
    }
};

#endif