#ifndef EDGE_H
#define EDGE_H


class Edge { // EDGE (for numbering)
public :
    int nMin;
    int nMax;
    int nE;
    int nO;

    Edge();
    Edge(const int, const int);
    Edge(const int, const int, const int);

    bool operator ==(const Edge&);
};


#endif // EDGE_H
