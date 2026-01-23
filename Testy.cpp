#include <gtest/gtest.h>
#include "include/Narzedzia.h"
#include "include/Drzewo.h"
#include "include/Pomiar.h"
#include "include/Analizator.h"

// --- CZESC 1: NARZEDZIA ---

TEST(NarzedziaTest, KonwersjaNaFloat_Poprawna) {
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat("123.45"), 123.45f);
}

TEST(NarzedziaTest, KonwersjaNaFloat_ZCudzyslowem) {
    EXPECT_FLOAT_EQ(Narzedzia::konwertujNaFloat("\"400.5\""), 400.5f);
}

TEST(NarzedziaTest, ParsowanieDaty_Poprawne) {
    DataCzas wynik = Narzedzia::parsujDate("15.05.2021 12:00");
    EXPECT_EQ(wynik.rok, 2021);
    EXPECT_EQ(wynik.miesiac, 5);
    EXPECT_EQ(wynik.dzien, 15);
}

TEST(NarzedziaTest, ParsowanieDaty_BlednyFormat) {
    DataCzas wynik = Narzedzia::parsujDate("BlednyString");
    EXPECT_EQ(wynik.rok, 2000);
}
// --- CZESC 2: DRZEWO I BAZA ---

TEST(BazaDanychTest, WyznaczanieCwiartki_Rano) {
    BazaDanych db;
    EXPECT_EQ(db.wyznaczIndeksCwiartki("01.01.2021 04:00"), 0);
}

TEST(BazaDanychTest, WyznaczanieCwiartki_Wieczor) {
    BazaDanych db;
    EXPECT_EQ(db.wyznaczIndeksCwiartki("01.01.2021 20:00"), 3);
}

TEST(DrzewoTest, SortowaniePomiary) {
    auto c = make_shared<Cwiartka>();
    auto p1 = make_shared<Pomiar>("08:15", 0, 0, 0, 0, 0);
    auto p2 = make_shared<Pomiar>("08:00", 0, 0, 0, 0, 0);

    c->dodajPomiar(p1);
    c->dodajPomiar(p2);

    EXPECT_EQ(c->pomiary[0]->czas, "08:00");
}

TEST(DrzewoTest, StrukturaHierarchii) {
    BazaDanych db;
    auto p = make_shared<Pomiar>("01.01.2021 08:00", 10, 20, 30, 40, 50);
    db.dodajDane(2021, 1, 1, p);

    auto dzien = db.lata[2021]->wezMiesiac(1)->wezDzien(1);
    EXPECT_FALSE(dzien->cwiartki.empty());
}