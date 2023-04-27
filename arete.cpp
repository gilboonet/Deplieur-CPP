#include <iostream>

#include "arete.h"

Arete::Arete() {}
Arete::Arete(const int n1, const int n2) {
    nMin = std::min(n1, n2);
    nMax = std::max(n1, n2);
    nA = -1;
}
Arete::Arete(const int n1, const int n2, const int n) {
    nMin = std::min(n1, n2);
    nMax = std::max(n1, n2);
    nA = n;
}
bool Arete::operator == (const Arete& a) {
    if (nMin == a.nMin && nMax == a.nMax)
        return true;
    return false;
}
