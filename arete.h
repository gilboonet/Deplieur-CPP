#ifndef ARETE_H
#define ARETE_H


class Arete { // ARETE (pour numérotation)
public :
    int nMin;
    int nMax;
    int nA;

    Arete();

    Arete(const int n1, const int n2);

    Arete(const int n1, const int n2, const int n);

    bool operator == (const Arete& a);
};

#endif // ARETE_H
