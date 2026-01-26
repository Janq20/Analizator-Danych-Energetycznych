#include "pch.h" 
#include "gtest/gtest.h"
#include <memory>
#include <string>

// --- DO£¥CZANIE TWOICH PLIKÓW ---
// Upewnij siê, ¿e œcie¿ka jest poprawna. 
// Jeœli ustawi³eœ katalogi w opcjach projektu, wystarczy samo "Narzedzia.h" itd.
#include "../Analizator-Danych-Energetycznych/Narzedzia.h"
#include "../Analizator-Danych-Energetycznych/Drzewo.h"
#include "../Analizator-Danych-Energetycznych/Analizator.h"
#include "../Analizator-Danych-Energetycznych/Pomiar.h"

// ==========================================
// 1. TESTY NARZÊDZI (Parsowanie i konwersja)
// ==========================================

TEST(NarzedziaTest, RozdzielanieTekstuCSV) {
    std::string linia = "2023-01-01 12:00,10.5,2.0";
    std::vector<std::string> wynik = Narzedzia::rozdzielTekst(linia, ',');

    ASSERT_EQ(wynik.size(), 3); // U¿ywamy ASSERT, bo jak to padnie, to dalej nie ma sensu sprawdzaæ
    EXPECT_EQ(wynik[0], "2023-01-01 12:00");
    EXPECT_EQ(wynik[1], "10.5");
    EXPECT_EQ(wynik[2], "2.0");
}

TEST(NarzedziaTest, KonwersjaFloat) {
    // Test poprawnej liczby
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat("123.45"), 123.45f);
    // Test liczby w cudzys³owie (czêste w CSV)
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat("\"99.9\""), 99.9f);
    // Test pustego stringa
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat(""), 0.0f);
    // Test œmieci
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat("abc"), 0.0f);
}

TEST(NarzedziaTest, ParsowanieDaty) {
    // Format z kropkami
    DataCzas dt1 = Narzedzia::parsujDate("01.05.2023 14:30");
    EXPECT_EQ(dt1.rok, 2023);
    EXPECT_EQ(dt1.miesiac, 5);
    EXPECT_EQ(dt1.dzien, 1);
    EXPECT_EQ(dt1.godzina, "14:30");

    // Format z myœlnikami (np. SQL)
    DataCzas dt2 = Narzedzia::parsujDate("2024-12-24 18:00");
    EXPECT_EQ(dt2.rok, 2024);
    EXPECT_EQ(dt2.miesiac, 12);
    EXPECT_EQ(dt2.dzien, 24);
}

// ==========================================
// 2. TESTY STRUKTURY DRZEWA (BazaDanych)
// ==========================================

TEST(DrzewoTest, WyznaczanieCwiartkiDnia) {
    BazaDanych db;
    // 00:00 - 05:59 -> Æwiartka 0
    EXPECT_EQ(db.wyznaczIndeksCwiartki("2023-01-01 02:00"), 0);
    // 06:00 - 11:59 -> Æwiartka 1
    EXPECT_EQ(db.wyznaczIndeksCwiartki("2023-01-01 06:00"), 1);
    EXPECT_EQ(db.wyznaczIndeksCwiartki("2023-01-01 11:59"), 1);
    // 12:00 - 17:59 -> Æwiartka 2
    EXPECT_EQ(db.wyznaczIndeksCwiartki("2023-01-01 13:00"), 2);
    // 18:00 - 23:59 -> Æwiartka 3
    EXPECT_EQ(db.wyznaczIndeksCwiartki("2023-01-01 19:30"), 3);
}

TEST(DrzewoTest, DodawanieDoStruktury) {
    BazaDanych db;
    auto pomiar = std::make_shared<Pomiar>("2023-05-10 07:00", 10, 20, 30, 40, 50);
    db.dodajDane(2023, 5, 10, pomiar);
    ASSERT_TRUE(db.lata.count(2023)); 
    ASSERT_TRUE(db.lata[2023]->miesiace.count(5));
    ASSERT_TRUE(db.lata[2023]->miesiace[5]->dni.count(10));
    auto cwiartka = db.lata[2023]->miesiace[5]->dni[10]->cwiartki[1];
    ASSERT_TRUE(cwiartka != nullptr);
    EXPECT_EQ(cwiartka->pomiary.size(), 1);
    EXPECT_EQ(cwiartka->pomiary[0]->autoKonsumpcja, 10.0f);
}

// ==========================================
// 3. TESTY ANALIZATORA (Logika biznesowa)
// ==========================================

class AnalizatorTest : public ::testing::Test {
protected:
    BazaDanych db;
    void SetUp() override {
        auto p1 = std::make_shared<Pomiar>("2023-01-01 10:00", 10.0f, 0, 0, 100.0f, 0);
        auto p2 = std::make_shared<Pomiar>("2023-01-01 11:00", 20.0f, 0, 0, 200.0f, 0);
        auto p3 = std::make_shared<Pomiar>("2023-01-01 12:00", 60.0f, 0, 0, 300.0f, 0); 

        db.dodajDane(2023, 1, 1, p1);
        db.dodajDane(2023, 1, 1, p2);
        db.dodajDane(2023, 1, 1, p3);
    }
};

TEST_F(AnalizatorTest, ObliczanieSumy) {
    Analizator anal(db);
    float suma = anal.obliczSume([](std::shared_ptr<Pomiar> p) {
        return p->autoKonsumpcja;
        });
    EXPECT_FLOAT_EQ(suma, 90.0f);
}

TEST_F(AnalizatorTest, ObliczanieSredniej) {
    Analizator anal(db);
    // Œrednia poboru: (100 + 200 + 300) / 3 = 200
    float srednia = anal.obliczSrednia([](std::shared_ptr<Pomiar> p) {
        return p->pobor;
        });
    EXPECT_FLOAT_EQ(srednia, 200.0f);
}

// ==========================================
// 4. TESTY ITERATORA
// ==========================================

TEST(IteratorTest, IteracjaPoWszystkichElementach) {
    BazaDanych db;
    db.dodajDane(2023, 1, 1, std::make_shared<Pomiar>("t1", 1, 0, 0, 0, 0));
    db.dodajDane(2023, 1, 1, std::make_shared<Pomiar>("t2", 1, 0, 0, 0, 0));

    Iterator it(db);
    int licznik = 0;
    while (!it.czyKoniec()) {
        licznik++;
        it.nastepny();
    }
    EXPECT_EQ(licznik, 2);
}