#ifndef IMODIFIER_H
#define IMODIFIER_H

#include <string>

// ================================================================
// IModifier — interface untuk semua enchantment/modifier
// ================================================================
// DIBUTUHKAN: RunSession mengiterasi vector<IModifier*> secara
// polimorfis (Decorator-like chain). Modifier baru bisa ditambah
// tanpa mengubah game loop (Open/Closed Principle).
// ================================================================
class IModifier {
public:
    virtual ~IModifier() = default;
    virtual int         applyModification(int currentScore) = 0;
    virtual std::string getName()        = 0;
    virtual std::string getDescription() = 0;
    virtual IModifier*  clone()          = 0;  // Prototype Pattern
};

// ================================================================
// ModifierA: ArcaneBoostModifier
// Efek: tambah nilai flat ke skor
// ================================================================
class ArcaneBoostModifier : public IModifier {
    int bonus_;
public:
    explicit ArcaneBoostModifier(int bonus) : bonus_(bonus) {}
    int applyModification(int score) override { return score + bonus_; }
    std::string getName()        override { return "Arcane Boost (+" + std::to_string(bonus_) + ")"; }
    std::string getDescription() override { return "Menambahkan " + std::to_string(bonus_) + " poin ke skor"; }
    IModifier*  clone()          override { return new ArcaneBoostModifier(bonus_); }
};

// ================================================================
// ModifierB: RuneAmplifierModifier
// Efek: kalikan skor dengan multiplier
// ================================================================
class RuneAmplifierModifier : public IModifier {
    int multiplier_;
public:
    explicit RuneAmplifierModifier(int m) : multiplier_(m) {}
    int applyModification(int score) override { return score * multiplier_; }
    std::string getName()        override { return "Rune Amplifier (x" + std::to_string(multiplier_) + ")"; }
    std::string getDescription() override { return "Mengalikan skor dengan " + std::to_string(multiplier_); }
    IModifier*  clone()          override { return new RuneAmplifierModifier(multiplier_); }
};

// ================================================================
// ModifierC: VoidSurgeModifier (SquareScoreModifier equivalent)
// Efek: mengkuadratkan skor (capped 9999)
// Ditambahkan sebagai bukti extensibility (analisis A2)
// ================================================================
class VoidSurgeModifier : public IModifier {
public:
    int applyModification(int score) override {
        int result = score * score;
        return (result > 9999) ? 9999 : result;
    }
    std::string getName()        override { return "Void Surge (Score^2)"; }
    std::string getDescription() override { return "Mengkuadratkan skor (maks 9999)"; }
    IModifier*  clone()          override { return new VoidSurgeModifier(); }
};

// ================================================================
// ModifierD: ElementalChainModifier
// Efek: bonus berdasarkan jumlah tile yang dimainkan
// ================================================================
class ElementalChainModifier : public IModifier {
    int bonusPerTile_;
    int tileCount_;
public:
    ElementalChainModifier(int bonusPerTile, int tileCount)
        : bonusPerTile_(bonusPerTile), tileCount_(tileCount) {}
    int applyModification(int score) override { return score + (bonusPerTile_ * tileCount_); }
    std::string getName()        override { return "Elemental Chain (+" + std::to_string(bonusPerTile_) + "/tile)"; }
    std::string getDescription() override { return "Bonus +" + std::to_string(bonusPerTile_) + " per tile dimainkan"; }
    IModifier*  clone()          override { return new ElementalChainModifier(bonusPerTile_, tileCount_); }
};

#endif