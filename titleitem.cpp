#include "titleitem.h"

//#include <QFocusEvent>
#include <QFont>
#include <QObject>
#include <QPen>
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsSceneMouseEvent>

#include "page.h"
#include "piece.h"
#include "unfold.h"

TitleItem::TitleItem(const QString text, Unfold *d) :
    QGraphicsSimpleTextItem(text), donnees(d)
{
    setFont(QFont("Courier", 15));
    setPen(QPen(Qt::blue, 1));
    setAcceptHoverEvents(true);
    setCursor(QCursor(Qt::OpenHandCursor));
    setFlags(QGraphicsItem::ItemIsFocusable
    | QGraphicsItem::ItemIsMovable
    | QGraphicsItem::ItemIsSelectable);

}

void TitleItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    int n = text().toInt()-1;
    donnees->setPieceCourante(n, this);
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
