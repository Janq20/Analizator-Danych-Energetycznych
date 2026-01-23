#include <iostream>
#include "include/Pomiar.h"
#include "include/Drzewo.h"
#include "include/Narzedzia.h"
#include "include/MenedzerPlikow.h"
#include "include/Analizator.h"

using namespace std;

int main() {
    // 1. Inicjalizacja Bazy Danych
    BazaDanych db;

    // 2. Inicjalizacja Menedżera Plików i Analizatora
    MenedzerPlikow menedzer;
    Analizator analizator(db);

    cout << "Witaj w Analizatorze Danych Energetycznych!" << endl;
    string nazwaPliku = "dane.csv";

    cout << "Proba wczytania pliku: " << nazwaPliku << endl;
    menedzer.wczytajPlik(nazwaPliku, db);
    analizator.wypiszStatystyki();

    cout << "\nKoniec programu." << endl;
    return 0;
}