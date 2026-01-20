#ifndef NARZEDZIA_H
#define NARZEDZIA_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm> 

using namespace std;

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

        try {
            return stof(tekst);
        }
        catch (...) {
            return 0.0f; 
        }
    }
};

#endif