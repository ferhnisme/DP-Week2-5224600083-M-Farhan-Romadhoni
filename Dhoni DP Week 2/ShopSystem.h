#ifndef SHOPSYSTEM_H
#define SHOPSYSTEM_H

#include <iostream>
#include <vector>
#include "ModifierFactory.h"

using namespace std;

// Struct untuk item di toko
struct ShopItem {
    int         modifierType;
    string      displayName;
    string      description;
    int         price;
};

// ================================================================
// ShopSystem — concrete class (tidak butuh interface)
// Hanya satu implementasi toko. Isi katalog bersifat mutable
// tanpa mengubah RunSession.
// ================================================================
class ShopSystem {
public:
    IModifier* visitShop(int& playerCoins, int tileCount = 0) {
        vector<ShopItem> catalog = {
            {1, "Arcane Boost I",    "Tambah +30 skor",          3},
            {2, "Arcane Boost II",   "Tambah +60 skor",          5},
            {3, "Rune Amplifier",    "Kalikan skor x2",          6},
            {4, "Grand Amplifier",   "Kalikan skor x3",          9},
            {5, "Void Surge",        "Kuadratkan skor (maks 9999)", 12},
            {6, "Elemental Chain",   "Bonus +15 per tile",       4},
        };

        cout << "\n||==================================||\n";
        cout << "||      RUNEFORGE ENCHANT SHOP    ||\n";
        cout << "||==================================||\n";
        cout << "  Koin kamu: " << playerCoins << "\n\n";

        for (size_t i = 0; i < catalog.size(); i++) {
            cout << "  [" << (i+1) << "] " << catalog[i].displayName
                 << " - " << catalog[i].description
                 << " | Harga: " << catalog[i].price << " koin\n";
        }
        cout << "  [0] Lewati toko\n";
        cout << "\n  Pilihan: ";

        int choice;
        cin >> choice;

        if (choice < 1 || choice > (int)catalog.size()) {
            cout << "  >> Kamu meninggalkan toko.\n";
            return nullptr;
        }

        ShopItem& item = catalog[choice - 1];
        if (playerCoins < item.price) {
            cout << "  >> Koin tidak cukup! (butuh " << item.price << ")\n";
            return nullptr;
        }

        playerCoins -= item.price;
        cout << "  >> [" << item.displayName << "] dibeli! Sisa koin: " << playerCoins << "\n";
        return ModifierFactory::createModifier(item.modifierType, tileCount);
    }
};

#endif