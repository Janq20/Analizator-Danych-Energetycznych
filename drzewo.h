/**
 * @file Drzewo.h
 * @brief Hierarchiczna struktura danych oraz iterator do nawigacji po pomiarach.
 *
 * Plik zawiera definicje klas tworzących drzewiastą strukturę bazy danych:
 * Rok -> Miesiąc -> Dzień -> Ćwiartka dnia -> Pomiary.
 */

#ifndef DRZEWO_H
#define DRZEWO_H

#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "Pomiar.h"

using namespace std;

// --- Struktura Drzewa ---

/**
 * @class Cwiartka
 * @brief Reprezentuje część dnia (okres 6-godzinny).
 *
 * Przechowuje wektor wskaźników na pomiary z danego okresu.
 */
class Cwiartka {
public:
    vector<shared_ptr<Pomiar>> pomiary; ///< Lista pomiarów w danej ćwiartce.

    /**
     * @brief Dodaje nowy pomiar do ćwiartki i sortuje listę chronologicznie.
     * @param p Wskaźnik na dodawany pomiar.
     */
    void dodajPomiar(shared_ptr<Pomiar> p) {
        pomiary.push_back(p);
        sort(pomiary.begin(), pomiary.end(), [](const shared_ptr<Pomiar>& a, const shared_ptr<Pomiar>& b) {
            return a->czas < b->czas;
            });
    }
};

/**
 * @class Dzien
 * @brief Reprezentuje pojedynczy dzień.
 *
 * Zawiera mapę ćwiartek dnia (kluczem jest indeks ćwiartki 0-3).
 */
class Dzien {
public:
    map<int, shared_ptr<Cwiartka>> cwiartki; ///< Mapa ćwiartek dnia.

    /**
     * @brief Zwraca wskaźnik na ćwiartkę o podanym numerze. Tworzy nową, jeśli nie istnieje.
     * @param numerCwiartki Indeks ćwiartki (0-3).
     * @return shared_ptr<Cwiartka> Wskaźnik na obiekt Ćwiartki.
     */
    shared_ptr<Cwiartka> wezCwiartke(int numerCwiartki) {
        if (cwiartki.find(numerCwiartki) == cwiartki.end()) {
            cwiartki[numerCwiartki] = make_shared<Cwiartka>();
        }
        return cwiartki[numerCwiartki];
    }
};

/**
 * @class Miesiac
 * @brief Reprezentuje miesiąc w roku.
 *
 * Zawiera mapę dni (kluczem jest numer dnia).
 */
class Miesiac {
public:
    map<int, shared_ptr<Dzien>> dni; ///< Mapa dni w miesiącu.

    /**
     * @brief Zwraca wskaźnik na dzień. Tworzy nowy, jeśli nie istnieje.
     * @param numerDnia Numer dnia (1-31).
     * @return shared_ptr<Dzien> Wskaźnik na obiekt Dnia.
     */
    shared_ptr<Dzien> wezDzien(int numerDnia) {
        if (dni.find(numerDnia) == dni.end()) {
            dni[numerDnia] = make_shared<Dzien>();
        }
        return dni[numerDnia];
    }
};

/**
 * @class Rok
 * @brief Reprezentuje rok kalendarzowy.
 *
 * Zawiera mapę miesięcy (kluczem jest numer miesiąca).
 */
class Rok {
public:
    map<int, shared_ptr<Miesiac>> miesiace; ///< Mapa miesięcy w roku.

    /**
     * @brief Zwraca wskaźnik na miesiąc. Tworzy nowy, jeśli nie istnieje.
     * @param numerMiesiaca Numer miesiąca (1-12).
     * @return shared_ptr<Miesiac> Wskaźnik na obiekt Miesiąca.
     */
    shared_ptr<Miesiac> wezMiesiac(int numerMiesiaca) {
        if (miesiace.find(numerMiesiaca) == miesiace.end()) {
            miesiace[numerMiesiaca] = make_shared<Miesiac>();
        }
        return miesiace[numerMiesiaca];
    }
};

/**
 * @class BazaDanych
 * @brief Główna klasa przechowująca całą strukturę danych pomiarowych.
 *
 * Struktura oparta jest na mapach zagnieżdżonych: Lata -> Miesiące -> Dni -> Ćwiartki.
 */
class BazaDanych {
public:
    map<int, shared_ptr<Rok>> lata; ///< Mapa lat dostępnych w bazie.

    /**
     * @brief Wyznacza indeks ćwiartki dnia na podstawie godziny w stringu.
     *
     * - 00:00 - 05:59 -> Ćwiartka 0
     * - 06:00 - 11:59 -> Ćwiartka 1
     * - 12:00 - 17:59 -> Ćwiartka 2
     * - 18:00 - 23:59 -> Ćwiartka 3
     *
     * @param czas Czas w formacie "YYYY-MM-DD HH:MM".
     * @return int Indeks ćwiartki (0-3).
     */
    int wyznaczIndeksCwiartki(string czas) {
        size_t spacja = czas.find(' ');
        if (spacja == string::npos) return 0;

        string godzinaStr = czas.substr(spacja + 1);
        int godzina = stoi(godzinaStr.substr(0, godzinaStr.find(':')));

        if (godzina < 6) return 0;
        if (godzina < 12) return 1;
        if (godzina < 18) return 2;
        return 3;
    }

    /**
     * @brief Dodaje pomiar do struktury drzewiastej.
     *
     * Metoda automatycznie tworzy brakujące węzły (lata, miesiące, dni).
     *
     * @param r Rok.
     * @param m Miesiąc.
     * @param d Dzień.
     * @param p Wskaźnik na obiekt Pomiaru.
     */
    void dodajDane(int r, int m, int d, shared_ptr<Pomiar> p) {
        if (lata.find(r) == lata.end()) {
            lata[r] = make_shared<Rok>();
        }

        int idx = wyznaczIndeksCwiartki(p->czas);
        lata[r]->wezMiesiac(m)->wezDzien(d)->wezCwiartke(idx)->dodajPomiar(p);
    }

    /**
     * @brief Spłaszcza całą strukturę drzewa do jednego wektora pomiarów.
     *
     * Metoda pomocnicza używana głównie przez Iterator.
     * @return vector<shared_ptr<Pomiar>> Wektor wszystkich pomiarów w bazie.
     */
    vector<shared_ptr<Pomiar>> pobierzWszystkiePomiary() {
        vector<shared_ptr<Pomiar>> wynik;
        for (auto const& [r, objRok] : lata) {
            for (auto const& [m, objMiesiac] : objRok->miesiace) {
                for (auto const& [d, objDzien] : objMiesiac->dni) {
                    for (auto const& [c, objCwiartka] : objDzien->cwiartki) {
                        wynik.insert(wynik.end(), objCwiartka->pomiary.begin(), objCwiartka->pomiary.end());
                    }
                }
            }
        }
        return wynik;
    }

    /**
     * @brief Metoda placeholder do wypisywania struktury (obecnie pusta).
     */
    void wypiszStrukture() {
    }
};

// --- Nowość: Klasa Iterator ---

/**
 * @class Iterator
 * @brief Umożliwia liniowe przechodzenie po wszystkich elementach bazy danych.
 *
 * Wzorzec projektowy Iterator. Ukrywa złożoność struktury drzewiastej przed użytkownikiem.
 */
class Iterator {
private:
    vector<shared_ptr<Pomiar>> dane; ///< Płaska lista pomiarów pobrana z bazy.
    size_t index;                    ///< Aktualna pozycja iteratora.

public:
    /**
     * @brief Konstruktor iteratora.
     *
     * Pobiera wszystkie dane z bazy do wektora i ustawia wskaźnik na początek.
     * @param db Referencja do bazy danych.
     */
    Iterator(BazaDanych& db) {
        dane = db.pobierzWszystkiePomiary();
        index = 0;
    }

    /**
     * @brief Sprawdza, czy iterator dotarł do końca zbioru danych.
     * @return true Jeśli nie ma więcej elementów.
     * @return false Jeśli są dostępne kolejne elementy.
     */
    bool czyKoniec() {
        return index >= dane.size();
    }

    /**
     * @brief Zwraca wskaźnik na obecny element (Pomiar).
     * @return shared_ptr<Pomiar> Wskaźnik na pomiar lub nullptr, jeśli koniec.
     */
    shared_ptr<Pomiar> obecny() {
        if (czyKoniec()) return nullptr;
        return dane[index];
    }

    /**
     * @brief Przesuwa iterator o jedną pozycję do przodu.
     */
    void nastepny() {
        index++;
    }

    /**
     * @brief Resetuje iterator na początek zbioru.
     */
    void naPoczatek() {
        index = 0;
    }
};

#endif