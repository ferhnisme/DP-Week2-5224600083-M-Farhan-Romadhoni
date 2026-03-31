#ifndef RUNETILE_H
#define RUNETILE_H

#include <string>
using namespace std;

enum class Element { FIRE, WATER, EARTH, WIND, VOID };

// RuneTile adalah unit data — tidak butuh interface
struct RuneTile {
    string name;
    Element     element;
    int         power;
};

inline string elementToString(Element e) {
    switch (e) {
        case Element::FIRE:  return "Fire";
        case Element::WATER: return "Water";
        case Element::EARTH: return "Earth";
        case Element::WIND:  return "Wind";
        case Element::VOID:  return "Void";
        default:             return "???";
    }
}

#endif