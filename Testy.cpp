#include <gtest/gtest.h>
#include "Narzedzia.h"
#include "Drzewo.h"
#include "Pomiar.h"
#include "Analizator.h"

// --- CZESC 1: NARZEDZIA ---

// 5. Test sumowania produkcji przez Analizator [cite: 19]
TEST(AnalizatorTest, ObliczanieSumyProdukcji) {
    BazaDanych db;
    db.dodajDane(2021, 10, 3, make_shared<Pomiar>("2021-10-03 10:15", 0, 0, 0, 0, 500));
    db.dodajDane(2021, 10, 3, make_shared<Pomiar>("2021-10-03 10:30", 0, 0, 0, 0, 300));
    Analizator an(db);
    float suma = an.obliczSume("2021-10-03 10:00", "2021-10-03 11:00", [](auto p){ return p->produkcja; });
    EXPECT_FLOAT_EQ(suma, 800.0f);
}

// 6. Test średniej z uwzględnieniem przedziału czasowego [cite: 19]
TEST(AnalizatorTest, ObliczanieSredniejWPrzedziale) {
    BazaDanych db;
    db.dodajDane(2021, 10, 3, make_shared<Pomiar>("2021-10-03 08:00", 0, 0, 0, 0, 200));
    db.dodajDane(2021, 10, 3, make_shared<Pomiar>("2021-10-03 20:00", 0, 0, 0, 0, 600)); // Poza zakresem testu
    Analizator an(db);
    float srednia = an.obliczSrednia("2021-10-03 07:00", "2021-10-03 09:00", [](auto p){ return p->produkcja; });
    EXPECT_FLOAT_EQ(srednia, 200.0f);
}

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
// --- CZESC 3: ANALIZA I ITERATOR ---

TEST(IteratorTest, IteracjaPodstawowa) {
    BazaDanych db;
    auto p1 = make_shared<Pomiar>("Czas1", 1, 1, 1, 1, 1);
    db.dodajDane(2021, 1, 1, p1);

    Iterator it(db);
    EXPECT_FALSE(it.czyKoniec());
    it.nastepny();
    EXPECT_TRUE(it.czyKoniec());
}

TEST(AnalizatorTest, SumowanieAutokonsumpcji) {
    BazaDanych db;
    auto p1 = make_shared<Pomiar>("t1", 100, 0, 0, 0, 0);
    auto p2 = make_shared<Pomiar>("t2", 50, 0, 0, 0, 0);
    db.dodajDane(2021, 1, 1, p1);
    db.dodajDane(2021, 1, 1, p2);

    Analizator analizator(db);
    float suma = analizator.obliczSume([](auto p) { return p->autoKonsumpcja; });

    EXPECT_FLOAT_EQ(suma, 150.0f);
}

TEST(AnalizatorTest, SumowaniePustejBazy) {
    BazaDanych db;
    Analizator analizator(db);
    float suma = analizator.obliczSume([](auto p) { return p->produkcja; });

    EXPECT_FLOAT_EQ(suma, 0.0f);
}