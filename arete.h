#ifndef ARETE_H
#define ARETE_H


class Arete { // ARETE(pour numérotation)
public :
    int nMin;
    int nMax;
    int nA;

    Arete();
    Arete(const int, const int);
    Arete(const int, const int, const int);

    bool operator ==(const Arete&);
};


#endif // ARETE_H
