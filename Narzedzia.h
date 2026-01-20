#ifndef NARZEDZIA_H
#define NARZEDZIA_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;
struct DataCzas {
    int rok;
    int miesiac;
    int dzien;
    string godzina;
};

class Narzedzia {
public:
    static vector<string> rozdzielTekst(string linia, char separator) {
        vector<string> wynik;
        stringstream ss(linia);
        string kawalek;
        while (getline(ss, kawalek, separator)) {
            wynik.push_back(kawalek);
        }
        return wynik;
    }

    static float konwertujNaFloat(string tekst) {
        tekst.erase(remove(tekst.begin(), tekst.end(), '\"'), tekst.end());
        if (tekst.empty()) return 0.0f;
        try { return stof(tekst); }
        catch (...) { return 0.0f; }
    }
    static DataCzas parsujDate(string surowaData) {
        auto czesci = rozdzielTekst(surowaData, ' ');
        if (czesci.size() < 2) return { 2000, 1, 1, "00:00" };

        string dataStr = czesci[0]; 
        string godzStr = czesci[1]; 
        auto dataLiczby = rozdzielTekst(dataStr, '.');
        if (dataLiczby.size() < 3) return { 2000, 1, 1, "00:00" };

        DataCzas wynik;
        wynik.dzien = stoi(dataLiczby[0]);
        wynik.miesiac = stoi(dataLiczby[1]);
        wynik.rok = stoi(dataLiczby[2]);
        wynik.godzina = godzStr;

        return wynik;
    }
};

#endif