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