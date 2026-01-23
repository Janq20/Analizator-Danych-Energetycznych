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
class Logger {
public:
    void logujBlad(string komunikat, string szczegoly) {
        cerr << "[LOG ERROR] " << komunikat << ": " << szczegoly << endl;
    }
};

class MenedzerPlikow {
private:
    Logger logger;

public:
    // --- WCZYTYWANIE CSV ---
    void wczytajPlik(const string& nazwaPliku, BazaDanych& db) {
        ifstream plik(nazwaPliku);
        if (!plik.is_open()) {
            cout << "BLAD: Nie mozna otworzyc pliku: " << nazwaPliku << endl;
            logger.logujBlad("Nieudane otwarcie CSV", nazwaPliku);
            return;
        }

        string linia;
        int licznik = 0, poprawne = 0;
        cout << "Wczytywanie CSV..." << endl;

        while (getline(plik, linia)) {
            licznik++;
            if (licznik == 1 || linia.empty()) continue;
            try {
                vector<string> dane = Narzedzia::rozdzielTekst(linia, ',');
                if (dane.size() < 6) throw runtime_error("Niekompletna linia");

                string czas = dane[0];
                float autok = Narzedzia::konwertujNaFloat(dane[1]);
                float eksp = Narzedzia::konwertujNaFloat(dane[2]);
                float imp = Narzedzia::konwertujNaFloat(dane[3]);
                float pob = Narzedzia::konwertujNaFloat(dane[4]);
                float prod = Narzedzia::konwertujNaFloat(dane[5]);

                DataCzas dt = Narzedzia::parsujDate(czas);
                auto pomiar = make_shared<Pomiar>(czas, autok, eksp, imp, pob, prod);
                db.dodajDane(dt.rok, dt.miesiac, dt.dzien, pomiar);
                poprawne++;
            }
            catch (...) {
                logger.logujBlad("Blad parsowania CSV", linia);
            }
        }
        plik.close();
        cout << "Wczytano CSV. Rekordow: " << poprawne << endl;
    }

    // --- ZAPIS BINARNY ---
    void zapiszBinarnie(const string& nazwaPliku, BazaDanych& db) {
        ofstream plik(nazwaPliku, ios::binary);
        if (!plik.is_open()) {
            cout << "Blad zapisu pliku binarnego!" << endl;
            return;
        }

        Iterator it(db);
        int licznik = 0;
        while (!it.czyKoniec()) {
            auto p = it.obecny();
            if (p) {
                int dlugoscDaty = p->czas.size();
                plik.write(reinterpret_cast<char*>(&dlugoscDaty), sizeof(dlugoscDaty));
                plik.write(p->czas.c_str(), dlugoscDaty);
                plik.write(reinterpret_cast<char*>(&p->autoKonsumpcja), sizeof(float));
                plik.write(reinterpret_cast<char*>(&p->eksportEnergii), sizeof(float));
                plik.write(reinterpret_cast<char*>(&p->importEnergii), sizeof(float));
                plik.write(reinterpret_cast<char*>(&p->pobor), sizeof(float));
                plik.write(reinterpret_cast<char*>(&p->produkcja), sizeof(float));
                licznik++;
            }
            it.nastepny();
        }
        plik.close();
        cout << "Zapisano binarnie " << licznik << " rekordow." << endl;
    }

    // --- ODCZYT BINARNY ---
    void wczytajBinarnie(const string& nazwaPliku, BazaDanych& db) {
        ifstream plik(nazwaPliku, ios::binary);
        if (!plik.is_open()) {
            cout << "Blad odczytu pliku binarnego!" << endl;
            return;
        }

        db = BazaDanych();
        int licznik = 0;
        while (plik.peek() != EOF) {
            int dlugoscDaty;
            if (!plik.read(reinterpret_cast<char*>(&dlugoscDaty), sizeof(dlugoscDaty))) break;

            string czas(dlugoscDaty, ' ');
            plik.read(&czas[0], dlugoscDaty);

            float a, e, i, pob, prod;
            plik.read(reinterpret_cast<char*>(&a), sizeof(float));
            plik.read(reinterpret_cast<char*>(&e), sizeof(float));
            plik.read(reinterpret_cast<char*>(&i), sizeof(float));
            plik.read(reinterpret_cast<char*>(&pob), sizeof(float));
            plik.read(reinterpret_cast<char*>(&prod), sizeof(float));

            DataCzas dt = Narzedzia::parsujDate(czas);
            auto pomiar = make_shared<Pomiar>(czas, a, e, i, pob, prod);
            db.dodajDane(dt.rok, dt.miesiac, dt.dzien, pomiar);
            licznik++;
        }
        plik.close();
        cout << "Wczytano binarnie " << licznik << " rekordow." << endl;
    }
};

#endif