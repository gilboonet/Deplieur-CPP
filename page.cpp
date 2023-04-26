#include "page.h"

Page::Page() {}
Page::Page(int pid) : id(pid) {}
void Page::ajoutePiece(Piece p) {
    pieces.push_back(p);
}
