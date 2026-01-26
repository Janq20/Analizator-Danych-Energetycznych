#include <chrono>
#include <iomanip>
#include <sstream>

class Logger {
private:
    string plikLog;
    string plikError;

    // Pobiera czas systemowy do nazwy pliku [cite: 34]
    string pobierzTimestamp() {
        auto teraz = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(teraz);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y%m%d_%H%M%S");
        return ss.str();
    }

public:
    Logger() {
        string ts = pobierzTimestamp();
        plikLog = "log_" + ts + ".txt";         // [cite: 32]
        plikError = "log_error_" + ts + ".txt"; // [cite: 32]
    }

    void loguj(string tresc, bool czyBlad = false) {
        ofstream fLog(plikLog, ios::app);
        fLog << tresc << endl;
        
        if (czyBlad) {
            ofstream fErr(plikError, ios::app);
            fErr << tresc << endl; // Zapisz tylko błędy do osobnego pliku [cite: 33]
        }
    }
};
void wczytajPlik(const string& nazwaPliku, BazaDanych& db) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        logger.loguj("BLAD: Nie mozna otworzyc pliku: " + nazwaPliku, true);
        return;
    }

    string linia;
    int sumaRekordow = 0;
    int poprawne = 0;
    int bledne = 0;

    while (getline(plik, linia)) {
        sumaRekordow++;
        if (sumaRekordow == 1) continue; // Pominiecie naglowka [cite: 4]
        if (linia.empty()) continue;     // Pominiecie pustych linii [cite: 31]

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
            logger.loguj("Poprawny rekord: " + linia);
        }
        catch (const exception& e) {
            bledne++;
            logger.loguj("BŁĄD (linia " + to_string(sumaRekordow) + "): " + linia, true);
        }
    }
    plik.close();

    // Podsumowanie na ekranie 
    cout << "--- PODSUMOWANIE WCZYTYWANIA ---" << endl;
    cout << "Poprawne rekordy: " << poprawne << endl;
    cout << "Bledne rekordy: " << bledne << endl;
    cout << "--------------------------------" << endl;
}
// Dodaj w klasie Logger
string generujNazweZCzasem(string prefiks) {
    auto teraz = time(nullptr);
    auto t = *localtime(&teraz);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y%m%d_%H%M%S", &t);
    return prefiks + "_" + string(buf) + ".txt";
}

void zapiszLogi(string msg, bool blad) {
    ofstream f1(nazwaLogu, ios::app); f1 << msg << endl;
    if(blad) { ofstream f2(nazwaBledu, ios::app); f2 << msg << endl; }
}

cout << "Podsumowanie: Poprawne: " << poprawne << ", Bledne: " << bledne << endl;