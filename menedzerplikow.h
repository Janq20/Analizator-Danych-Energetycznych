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