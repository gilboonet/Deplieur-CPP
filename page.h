#ifndef PAGE_H
#define PAGE_H
#include "piece.h"
#include <vector>

class Page {
public :
    int id;
    std::vector<Piece> pieces;

    Page();
    Page(int pid);
    void ajoutePiece(Piece p);
};

#endif // PAGE_H
