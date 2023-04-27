#ifndef PAGE_H
#define PAGE_H


#include <vector>

#include "piece.h"

class Page {
public :
    int id;
    std::vector<Piece> pieces;

    Page ();
    Page (int pid);

    void ajoutePiece (const Piece &p);
};


#endif // PAGE_H
