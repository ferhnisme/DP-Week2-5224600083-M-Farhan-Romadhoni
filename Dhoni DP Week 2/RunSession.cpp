#include "RunSession.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

namespace {
vector<RuneTile> buildTilePool() {
    return {
        {"Ember",    Element::FIRE,  8},
        {"Blaze",    Element::FIRE,  12},
        {"Inferno",  Element::FIRE,  16},
        {"Spark",    Element::FIRE,  6},
        {"Tide",     Element::WATER, 8},
        {"Current",  Element::WATER, 10},
        {"Abyss",    Element::WATER, 14},
        {"Drizzle",  Element::WATER, 6},
        {"Pebble",   Element::EARTH, 7},
        {"Boulder",  Element::EARTH, 13},
        {"Mountain", Element::EARTH, 18},
        {"Dust",     Element::EARTH, 5},
        {"Gust",     Element::WIND,  7},
        {"Tornado",  Element::WIND,  15},
        {"Breeze",   Element::WIND,  5},
        {"Cyclone",  Element::WIND,  11},
        {"Rift",     Element::VOID,  20},
        {"Shade",    Element::VOID,  17},
        {"Eclipse",  Element::VOID,  22},
        {"Shadow",   Element::VOID,  9},
    };
}
}

RunSession::RunSession()
    : totalScore(0),
      targetScore(200),
      castsRemaining(4),
      discardRemaining(3),
      roundNumber(1),
      coins(5) {
    srand(static_cast<unsigned int>(time(nullptr)));
}

RunSession::~RunSession() {
    for (IModifier* modifier : activeModifiers) {
        delete modifier;
    }
}

void RunSession::fillHand() {
    const vector<RuneTile> pool = buildTilePool();
    while (static_cast<int>(currentHand.size()) < 7) {
        int idx = rand() % static_cast<int>(pool.size());
        currentHand.push_back(pool[idx]);
    }
}

void RunSession::displayHand() const {
    cout << "\n  Rune tiles di tangan:\n  ";
    for (const RuneTile& tile : currentHand) {
        cout << setw(13) << left
                  << (tile.name + "[" + elementToString(tile.element).substr(0, 1) + "]");
    }
    cout << "\n  ";
    for (size_t i = 0; i < currentHand.size(); ++i) {
        cout << setw(13) << left
                  << ("(" + to_string(i + 1) + ")pw=" + to_string(currentHand[i].power));
    }
    cout << "\n";
}

void RunSession::displayStatus() {
    cout << "\n|=====================================|\n";
    cout << "| Round: " << setw(2) << roundNumber
              << "  Target: " << setw(5) << targetScore
              << "  Koin: " << setw(3) << coins << " |\n";
    cout << "| Skor : " << setw(2) << totalScore
              << "  Cast : " << setw(5) << castsRemaining
              << "   Buang: " << setw(2) << discardRemaining << " |\n";

    if (!activeModifiers.empty()) {
        cout << "| Modifier aktif:                     |\n";
        for (IModifier* modifier : activeModifiers) {
            cout << "| - " << setw(34) << left << modifier->getName() << "|\n";
        }
    }
    cout << "|=====================================|\n";
}

int RunSession::applyModifiers(int baseScore) {
    int score = baseScore;
    if (activeModifiers.empty()) {
        return score;
    }

    cout << "\n  [Modifier Chain]\n";
    for (IModifier* modifier : activeModifiers) {
        int before = score;
        score = modifier->applyModification(score);
        cout << "   " << modifier->getName() << ": " << before << " -> " << score << "\n";
    }
    return score;
}

vector<RuneTile> RunSession::selectTilesFromInput(vector<int>& selectedIndices) {
    selectedIndices.clear();
    vector<RuneTile> selectedTiles;

    cout << "\n  Pilih tile (nomor dipisah spasi): ";
    string line;
    getline(cin >> ws, line);
    stringstream parser(line);

    int idx = 0;
    while (parser >> idx) {
        if (idx >= 1 && idx <= static_cast<int>(currentHand.size()) &&
            find(selectedIndices.begin(), selectedIndices.end(), idx) == selectedIndices.end()) {
            selectedIndices.push_back(idx);
            selectedTiles.push_back(currentHand[idx - 1]);
        }
    }

    return selectedTiles;
}

void RunSession::enterShop() {
    IModifier* purchasedModifier = shopSystem.visitShop(coins, static_cast<int>(currentHand.size()));
    if (purchasedModifier != nullptr) {
        activeModifiers.push_back(purchasedModifier);
    }
}

void RunSession::playRound() {
    currentHand.clear();
    fillHand();

    while (castsRemaining > 0 && totalScore < targetScore) {
        displayStatus();
        displayHand();

        vector<int> selectedIndices;
        vector<RuneTile> selectedTiles = selectTilesFromInput(selectedIndices);
        if (selectedTiles.empty()) {
            cout << "  Tidak ada tile valid yang dipilih.\n";
            continue;
        }

        cout << "\n  [1] Cast Spell  [2] Buang\n  Pilih: ";
        int action = 0;
        cin >> action;

        if (action == 2) {
            if (discardRemaining == 0) {
                cout << "  Kesempatan buang habis.\n";
                continue;
            }
            --discardRemaining;
            cout << "  " << selectedTiles.size() << " tile dibuang.\n";
        } else {
            string spellName;
            int baseScore = scoringSystem.evaluateSpell(selectedTiles, spellName);
            int finalScore = applyModifiers(baseScore);

            cout << "\n  Spell       : " << spellName << "\n";
            cout << "  Base Score  : " << baseScore << "\n";
            cout << "  Final Score : " << finalScore << "\n";

            totalScore += finalScore;
            --castsRemaining;
            cout << "  Total Score : " << totalScore << " / " << targetScore << "\n";
        }

        sort(selectedIndices.rbegin(), selectedIndices.rend());
        for (int index : selectedIndices) {
            currentHand.erase(currentHand.begin() + index - 1);
        }
        fillHand();
    }
}

void RunSession::startRun() {
    cout << "\n||=======================================||\n";
    cout << "||               RUNEFORGE               ||\n";
    cout << "||      Core Loop Skeleton - Task 2      ||\n";
    cout << "||=======================================||\n";

    for (int round = 0; round < 3; ++round) {
        totalScore = 0;
        castsRemaining = 4;
        discardRemaining = 3;

        cout << "\n=======================================\n";
        cout << "  ROUND " << roundNumber << " | Target: " << targetScore << "\n";
        cout << "=======================================\n";

        playRound();

        if (totalScore < targetScore) {
            cout << "\n  GAME OVER - target ronde tidak tercapai.\n";
            cout << "  Final Score: " << totalScore << " / " << targetScore << "\n";
            return;
        }

        int reward = 3 + castsRemaining;
        coins += reward;

        cout << "\n  ROUND " << roundNumber << " CLEARED\n";
        cout << "  Reward koin: " << reward << " | Total koin: " << coins << "\n";

        enterShop();

        ++roundNumber;
        targetScore += 300;
    }

    cout << "\n||=======================================||\n";
    cout << "||         RUN BERHASIL DITUNTASKAN      ||\n";
    cout << "||=======================================||\n";
}
