#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "unfold.h"
#include "dialognouveau.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Unfold *unfold;

private slots:
    void nouveau();
    void lanceNouveau();

    void ouvrir();
    void sauver();
    void quitter();
    void exporterSVG();

    void unlockMenus();
    void basculerOptimiserNums();

    void tournerD();
    void tournerG();
    void tournerD05();
    void tournerG05();
    void tournerD15();
    void tournerG15();

    void monter();
    void descendre();
    void droite();
    void gauche();

    void zoomNormal();
    void zoomPlus();
    void zoomMoins();

    void pageFormat_A3();
    void pageFormat_A4();
    void pageFormat_A5();
    void pageFormat_Cr1();
    void pageFormat_Cr2();
    void pageFormat_Cr3();
    void pageFormat_Cr4();
    void pageFormat(int);

    void langMode0();
    void langMode1();
    void langMode2();
    void langModeSet(int);

    void langH11();
    void langH12();
    void langH13();
    void langH14();
    void langH15();
    void langH16();
    void langH17();
    void langH18();
    void langH19();
    void langH20();
    void langHSet(int);

    void openWikiPage();
    void openVideosPage();

private:
    Ui::MainWindow *ui;
    //QGraphicsScene *scene;
    DialogNouveau *dN;
    QGraphicsView *vue;
};

#endif // MAINWINDOW_H
