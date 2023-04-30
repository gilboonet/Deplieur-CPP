#include "page.h"

Page::Page() {}
Page::Page(int pid) : id(pid) {}
void Page::ajoutePiece(const Piece& p) {
    pieces.push_back(p);
}
