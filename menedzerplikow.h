#ifndef MENEDZER_PLIKOW_H
#define MENEDZER_PLIKOW_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include "Drzewo.h"
#include "Narzedzia.h"
#include "Pomiar.h"

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
        int poprawne = 0;
        int bledne = 0;

        cout << "Wczytywanie danych..." << endl;

        while (getline(plik, linia)) {
            licznikLinii++;
            if (licznikLinii == 1) continue;
            if (linia.empty()) continue;

            try {
                vector<string> dane = Narzedzia::rozdzielTekst(linia, ',');

                if (dane.size() < 6) {
                    throw runtime_error("Niekompletna linia");
                }

                string czas = dane[0];
                float autoKonsumpcja = Narzedzia::konwertujNaFloat(dane[1]);
                float eksport = Narzedzia::konwertujNaFloat(dane[2]);
                float importEn = Narzedzia::konwertujNaFloat(dane[3]);
                float pobor = Narzedzia::konwertujNaFloat(dane[4]);
                float produkcja = Narzedzia::konwertujNaFloat(dane[5]);

                DataCzas dt = Narzedzia::parsujDate(czas);

                auto pomiar = make_shared<Pomiar>(czas, autoKonsumpcja, eksport, importEn, pobor, produkcja);
                db.dodajDane(dt.rok, dt.miesiac, dt.dzien, pomiar);

                poprawne++;

            }
            catch (...) {
                bledne++;
                logger.logujBlad("Blad parsowania linii", linia);
            }
        }

        plik.close();

        cout << "=== Raport wczytywania ===" << endl;
        cout << "Linii ogolem: " << licznikLinii << endl;
        cout << "Poprawne:     " << poprawne << endl;
        cout << "Bledne:       " << bledne << endl;
    }
};

#endif