#ifndef RUNSESSION_H
#define RUNSESSION_H

#include <vector>
#include <string>
#include "RuneTile.h"
#include "ScoringSystem.h"
#include "IModifier.h"
#include "ShopSystem.h"
using namespace std;

class RunSession {
private:
    int totalScore;
    int targetScore;
    int castsRemaining;
    int discardRemaining;
    int roundNumber;
    int coins;

    vector<RuneTile> currentHand;
    vector<IModifier*> activeModifiers;

    ScoringSystem scoringSystem;
    ShopSystem shopSystem;

    void fillHand();
    void displayHand() const;
    void displayStatus();
    int applyModifiers(int baseScore);
    vector<RuneTile> selectTilesFromInput(vector<int>& selectedIndices);

public:
    RunSession();
    ~RunSession();

    void startRun();
    void playRound();
    void enterShop();
};

#endif
