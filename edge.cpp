#include <iostream>

#include "edge.h"

Edge::Edge() {}
Edge::Edge(const int n1, const int n2) :
    nMin(std::min(n1, n2)),  nMax(std::max(n1, n2)),nE(-1), nO(-1) {}
Edge::Edge(const int n1, const int n2, const int n) :
    nMin(std::min(n1, n2)), nMax(std::max(n1, n2)), nE(n), nO(-1) {}
bool Edge::operator ==(const Edge& e) const {
    if(nMin == e.nMin && nMax == e.nMax)
        return true;
    return false;
}
