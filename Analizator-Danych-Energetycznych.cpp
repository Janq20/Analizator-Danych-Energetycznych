#include <iostream>
#include <memory>
#include "include/Pomiar.h"
#include "include/Drzewo.h"

using namespace std;

int main() {
    cout << "Testowanie struktury drzewiastej..." << endl;

    // 1. Inicjalizacja bazy
    BazaDanych db;
    // Dane
    auto p1 = make_shared<Pomiar>("01.10.2020 0:00", 0, 0, 406.8, 406.8, 0);
    auto p2 = make_shared<Pomiar>("01.10.2020 0:15", 0, 0, 403.5, 403.5, 0);
    auto p3 = make_shared<Pomiar>("15.05.2021 12:00", 50, 100, 0, 200, 300);

    cout << "Dodaje dane..." << endl;
    db.dodajDane(2020, 10, 1, p1);
    db.dodajDane(2020, 10, 1, p2);
    db.dodajDane(2021, 5, 15, p3); 
    db.wypiszStrukture();
    cout << "Szczegoly pierwszego pomiaru:" << endl;
    p1->wypisz();

    return 0;
}