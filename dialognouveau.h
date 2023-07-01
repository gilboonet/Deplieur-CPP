#ifndef DIALOGNOUVEAU_H
#define DIALOGNOUVEAU_H

#include <QDialog>

namespace Ui {
class DialogNouveau;
}

struct RN_rep {
    int a;
    qreal b;
};

class DialogNouveau : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNouveau(QWidget *parent = nullptr);
    ~DialogNouveau();

    RN_rep retourneEdite();

private:
    Ui::DialogNouveau *ui;

};

#endif // DIALOGNOUVEAU_H
