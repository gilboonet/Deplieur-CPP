#include "pieceitem.h"

#include "unfold.h"

PieceItem::PieceItem(const QPolygonF &polygon, int nPiece, Unfold *d) :
    QGraphicsPolygonItem(polygon), donnees(d), num(nPiece)
{
    setCursor(QCursor(Qt::OpenHandCursor));
    setFlags(QGraphicsItem::ItemIsMovable);
    setPen(Qt::NoPen);
    setZValue(0);
}

void PieceItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    //int n = text().toInt()-1;
    //int n = num;
    donnees->setPieceCourante(num, this);
    Piece *pieceCourante = donnees->getPieceCourante();

    Page* pageCourante = nullptr;
    for (auto&& pg : donnees->pages) {
        for (auto&& p : pg.pieces) {
            if (p == *pieceCourante) {
                pageCourante = &pg;
                break;
            }
        }
        if (pageCourante)
            break;
    }
}
