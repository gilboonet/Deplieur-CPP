#include "numitem.h"

#include <QCursor>
#include <QFont>
#include <QFontDatabase>

NumItem::NumItem(const QString text, int id1, int id2)
: QGraphicsSimpleTextItem(text), nF(id1), fid(id2)
{
    setCursor(Qt::PointingHandCursor);
    setAcceptHoverEvents(true);
    const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    setFont(QFont(fixedFont.family(), 9));
}

void NumItem::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    TitleItem* ti = static_cast<TitleItem*>(parentItem());
    ti->donnees->changeLanguette(nF, fid);
    //qInfo() << "customclic " << nF << " " << fid;
}
