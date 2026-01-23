#include <iostream>
#include <string>
#include <limits> 
#include "Pomiar.h"
#include "Drzewo.h"
#include "Narzedzia.h"
#include "MenedzerPlikow.h"
#include "Analizator.h"

using namespace std;

void wyswietlMenu() {
    cout << "\n==========================================" << endl;
    cout << "   ANALIZATOR DANYCH ENERGETYCZNYCH v1.0" << endl;
    cout << "==========================================" << endl;
    cout << "1. Wczytaj dane z pliku CSV" << endl;
    cout << "2. Wykonaj analize danych (Statystyki)" << endl;
    cout << "3. Zapisz dane do pliku binarnego" << endl;
    cout << "4. Wczytaj dane z pliku binarnego" << endl;
    cout << "5. Wyjscie" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Wybierz opcje: ";
}

int main() {
    BazaDanych db;
    MenedzerPlikow menedzer;
    Analizator analizator(db);

    int wybor = 0;
    string nazwaPliku;

    while (true) {
        wyswietlMenu();
        if (!(cin >> wybor)) {
            cout << "To nie jest liczba! Sprobuj ponownie." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (wybor) {
        case 1:
            cout << "Podaj nazwe pliku CSV (np. Chart Export.csv): ";
            cin >> nazwaPliku;
            menedzer.wczytajPlik(nazwaPliku, db);
            break;
        case 2:
            analizator.wypiszStatystyki();
            break;
        case 3:
            cout << "Podaj nazwe pliku do zapisu (np. baza.bin): ";
            cin >> nazwaPliku;
            menedzer.zapiszBinarnie(nazwaPliku, db);
            break;
        case 4:
            cout << "Podaj nazwe pliku do odczytu (np. baza.bin): ";
            cin >> nazwaPliku;
            menedzer.wczytajBinarnie(nazwaPliku, db);
            break;
        case 5:
            cout << "Zamykanie programu..." << endl;
            return 0;
        default:
            cout << "Nieznana opcja." << endl;
        }
    }
    return 0;
}