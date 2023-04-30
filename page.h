#ifndef PAGE_H
#define PAGE_H


#include <vector>

#include "piece.h"

class Page {
public :
    int id;
    std::vector<Piece> pieces;

    Page();
    Page(int);

    void ajoutePiece(const Piece &);
};


#endif // PAGE_H
