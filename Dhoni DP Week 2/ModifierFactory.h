#ifndef MODIFIERFACTORY_H
#define MODIFIERFACTORY_H

#include "IModifier.h"

class ModifierFactory {
public:
    static IModifier* createModifier(int type, int tileCount = 0) {
        switch (type) {
            case 1: return new ArcaneBoostModifier(30);
            case 2: return new ArcaneBoostModifier(60);
            case 3: return new RuneAmplifierModifier(2);
            case 4: return new RuneAmplifierModifier(3);
            case 5: return new VoidSurgeModifier();
            case 6: return new ElementalChainModifier(15, tileCount);
            default: return nullptr;
        }
    }
};

#endif
