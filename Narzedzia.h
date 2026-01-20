#ifndef NARZEDZIA_H
#define NARZEDZIA_H

#include <string>
#include <vector>
#include <sstream>

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
};

#endif