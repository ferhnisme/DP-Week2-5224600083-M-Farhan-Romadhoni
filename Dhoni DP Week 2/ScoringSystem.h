#ifndef SCORINGSYSTEM_H
#define SCORINGSYSTEM_H

#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include "RuneTile.h"

using namespace std;

// ================================================================
// IScoringStrategy — Strategy Pattern (Behavioral)
// ================================================================
// DIBUTUHKAN: ada banyak kombinasi rune yang mungkin, masing-masing
// punya aturan skor berbeda. Strategy memisahkan aturan scoring
// dari ScoringSystem sehingga mudah ditambah tanpa mengubah sistem.
// ================================================================
class IScoringStrategy {
public:
    virtual ~IScoringStrategy() = default;
    virtual int    calculateScore(const vector<RuneTile>& tiles) = 0;
    virtual string getSpellName() = 0;
};

// ── Semua elemen sama → Elemental Convergence ───────────────────
class ElementalConvergenceStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        if (tiles.size() < 3) return 0;
        Element first = tiles[0].element;
        for (const auto& t : tiles)
            if (t.element != first) return 0;
        int total = 0;
        for (const auto& t : tiles) total += t.power;
        return total * 3; // bonus x3 karena semua sama
    }
    string getSpellName() override { return "Elemental Convergence"; }
};

// ── Ada VOID di antara tiles → Void Rift ────────────────────────
class VoidRiftStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        bool hasVoid = false;
        int total = 0;
        for (const auto& t : tiles) {
            if (t.element == Element::VOID) hasVoid = true;
            total += t.power;
        }
        if (!hasVoid) return 0;
        return total + 80;
    }
    string getSpellName() override { return "Void Rift"; }
};

// ── 4 elemen berbeda → Primal Surge ─────────────────────────────
class PrimalSurgeStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        if (tiles.size() < 4) return 0;
        map<Element, int> counts;
        for (const auto& t : tiles) counts[t.element]++;
        if (counts.size() < 4) return 0;
        int total = 0;
        for (const auto& t : tiles) total += t.power;
        return total + 120;
    }
    string getSpellName() override { return "Primal Surge"; }
};

// ── Semua power sama → Harmonic Pulse ───────────────────────────
class HarmonicPulseStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        if (tiles.size() < 2) return 0;
        int firstPow = tiles[0].power;
        for (const auto& t : tiles)
            if (t.power != firstPow) return 0;
        return (firstPow * (int)tiles.size()) + 60;
    }
    string getSpellName() override { return "Harmonic Pulse"; }
};

// ── FIRE + WATER bersama → Steam Burst ──────────────────────────
class SteamBurstStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        bool hasFire = false, hasWater = false;
        int total = 0;
        for (const auto& t : tiles) {
            if (t.element == Element::FIRE)  hasFire  = true;
            if (t.element == Element::WATER) hasWater = true;
            total += t.power;
        }
        if (!hasFire || !hasWater) return 0;
        return total + 50;
    }
    string getSpellName() override { return "Steam Burst"; }
};

// ── Default: jumlah power saja ───────────────────────────────────
class BasicSpellStrategy : public IScoringStrategy {
public:
    int calculateScore(const vector<RuneTile>& tiles) override {
        int total = 0;
        for (const auto& t : tiles) total += t.power;
        return total + 10;
    }
    string getSpellName() override { return "Basic Spell"; }
};

// ================================================================
// ScoringSystem — concrete class (tidak butuh interface)
// Hanya satu sistem scoring; tidak perlu diganti polimorfis.
// ================================================================
class ScoringSystem {
public:
    int evaluateSpell(const vector<RuneTile>& tiles, string& outSpellName) {
        // Coba dari kombinasi terkuat ke terlemah
        vector<IScoringStrategy*> strategies = {
            new ElementalConvergenceStrategy(),
            new PrimalSurgeStrategy(),
            new VoidRiftStrategy(),
            new HarmonicPulseStrategy(),
            new SteamBurstStrategy(),
            new BasicSpellStrategy()
        };

        int finalScore = 0;
        for (auto s : strategies) {
            int score = s->calculateScore(tiles);
            if (score > 0) {
                outSpellName = s->getSpellName();
                finalScore   = score;
                break;
            }
        }
        for (auto s : strategies) delete s;
        return finalScore;
    }
};

#endif