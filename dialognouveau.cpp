#include "dialognouveau.h"
#include "ui_dialognouveau.h"

DialogNouveau::DialogNouveau(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogNouveau)
{
    ui->setupUi(this);

    QStringList fp = {
        "A3 : 29.7 x 42   cm",
        "A4 : 21   x 29.7 cm",
        "A5 : 18.8 x 21   cm",
        "C1 : 30.5 x 61   cm",
        "C2 : 30.5 x 30.5 cm",
        "C3 : 11.5 x 30.5 cm",
        "C4 : 11.5 x 16.5 cm"
    };
    ui->comboBox_FormatPage->addItems(fp);
    ui->comboBox_FormatPage->setCurrentIndex(1);
    ui->lineEdit_Echelle->setText("1");
}

DialogNouveau::~DialogNouveau()
{
    delete ui;
}

RN_rep DialogNouveau::retourneEdite() {
    RN_rep r;
    r.a = ui->comboBox_FormatPage->currentIndex();
    r.b = ui->lineEdit_Echelle->text().toFloat();
    return r;
}
