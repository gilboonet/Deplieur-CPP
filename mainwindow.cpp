#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "lineitem.h"

#include <fstream>
#include <QActionGroup>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsView>
#include <QGridLayout>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGridLayout *gridLayout;
    gridLayout = new QGridLayout(centralWidget());
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    vue = new QGraphicsView();
    vue->setInteractive(true);
    vue->setDragMode(QGraphicsView::RubberBandDrag);
    vue->setRubberBandSelectionMode(Qt::IntersectsItemShape);
    gridLayout->addWidget(vue);

    zoomNormal();

    connect(ui->actionOuvrir, &QAction::triggered, this, &MainWindow::ouvrir);
    connect(ui->actionNouveau, &QAction::triggered, this, &MainWindow::nouveau);
    connect(ui->actionSauver, &QAction::triggered, this, &MainWindow::sauver);
    connect(ui->actionQuitter, &QAction::triggered, this, &MainWindow::quitter);
    connect(ui->actionExporter, &QAction::triggered, this, &MainWindow::exporterSVG);

    connect(ui->actionZoom_Normal, &QAction::triggered, this, &MainWindow::zoomNormal);
    connect(ui->actionZoom_Plus, &QAction::triggered, this, &MainWindow::zoomPlus);
    connect(ui->actionZoom_Moins, &QAction::triggered, this, &MainWindow::zoomMoins);

    connect(ui->actionTourner_1_Droite, &QAction::triggered, this, &MainWindow::tournerD);
    connect(ui->actionTourner_1_Gauche, &QAction::triggered, this, &MainWindow::tournerG);
    connect(ui->actionTourner_5_Droite, &QAction::triggered, this, &MainWindow::tournerD05);
    connect(ui->actionTourner_5_Gauche, &QAction::triggered, this, &MainWindow::tournerG05);
    connect(ui->actionTourner_15_Droite, &QAction::triggered, this, &MainWindow::tournerD15);
    connect(ui->actionTourner_15_Gauche, &QAction::triggered, this, &MainWindow::tournerG15);

    connect(ui->actionMonter, &QAction::triggered, this, &MainWindow::monter);
    connect(ui->actionDescendre, &QAction::triggered, this, &MainWindow::descendre);
    connect(ui->actionGauche, &QAction::triggered, this, &MainWindow::gauche);
    connect(ui->actionDroite, &QAction::triggered, this, &MainWindow::droite);

    connect(ui->actionOptimiser_num, &QAction::triggered, this, &MainWindow::basculerOptimiserNums);

    QActionGroup* PageFormatGroup = new QActionGroup(this);
    PageFormatGroup->addAction(ui->actionA3);
    PageFormatGroup->addAction(ui->actionA4);
    PageFormatGroup->addAction(ui->actionA5);
    PageFormatGroup->addAction(ui->actionCr1);
    PageFormatGroup->addAction(ui->actionCr2);
    PageFormatGroup->addAction(ui->actionCr3);
    PageFormatGroup->addAction(ui->actionCr4);
    ui->actionA4->setChecked(true);

    connect(ui->actionA3, &QAction::toggled, this, &MainWindow::pageFormat_A3);
    connect(ui->actionA4, &QAction::toggled, this, &MainWindow::pageFormat_A4);
    connect(ui->actionA5, &QAction::toggled, this, &MainWindow::pageFormat_A5);
    connect(ui->actionCr1, &QAction::toggled, this, &MainWindow::pageFormat_Cr1);
    connect(ui->actionCr2, &QAction::toggled, this, &MainWindow::pageFormat_Cr2);
    connect(ui->actionCr3, &QAction::toggled, this, &MainWindow::pageFormat_Cr3);
    connect(ui->actionCr4, &QAction::toggled, this, &MainWindow::pageFormat_Cr4);

    QActionGroup* ModeLanguettesGroup = new QActionGroup(this);
    ModeLanguettesGroup->addAction(ui->actionLang0);
    ModeLanguettesGroup->addAction(ui->actionLang1);
    ModeLanguettesGroup->addAction(ui->actionLang2);
    ui->actionLang0->setChecked(true);

    connect(ui->actionLang0, &QAction::toggled, this, &MainWindow::langMode0);
    connect(ui->actionLang1, &QAction::toggled, this, &MainWindow::langMode1);
    connect(ui->actionLang2, &QAction::toggled, this, &MainWindow::langMode2);

    ui->menuAffichage->setEnabled(false);
    ui->menuGabarit->setEnabled(false);
    ui->menuPiece->setEnabled(false);

    QActionGroup* HLanguettesGroup = new QActionGroup(this);
    HLanguettesGroup->addAction(ui->actHL_11);
    HLanguettesGroup->addAction(ui->actHL_12);
    HLanguettesGroup->addAction(ui->actHL_13);
    HLanguettesGroup->addAction(ui->actHL_14);
    HLanguettesGroup->addAction(ui->actHL_15);
    HLanguettesGroup->addAction(ui->actHL_16);
    HLanguettesGroup->addAction(ui->actHL_17);
    HLanguettesGroup->addAction(ui->actHL_18);
    HLanguettesGroup->addAction(ui->actHL_19);
    HLanguettesGroup->addAction(ui->actHL_20);

    ui->actHL_15->setChecked(true);

    connect(ui->actHL_11, &QAction::toggled, this, &MainWindow::langH11);
    connect(ui->actHL_12, &QAction::toggled, this, &MainWindow::langH12);
    connect(ui->actHL_13, &QAction::toggled, this, &MainWindow::langH13);
    connect(ui->actHL_14, &QAction::toggled, this, &MainWindow::langH14);
    connect(ui->actHL_15, &QAction::toggled, this, &MainWindow::langH15);
    connect(ui->actHL_16, &QAction::toggled, this, &MainWindow::langH16);
    connect(ui->actHL_17, &QAction::toggled, this, &MainWindow::langH17);
    connect(ui->actHL_18, &QAction::toggled, this, &MainWindow::langH18);
    connect(ui->actHL_19, &QAction::toggled, this, &MainWindow::langH19);
    connect(ui->actHL_20, &QAction::toggled, this, &MainWindow::langH20);

    connect(ui->actionWiki, &QAction::triggered, this, &MainWindow::openWikiPage);
    connect(ui->actionVideos, &QAction::triggered, this, &MainWindow::openVideosPage);

    unfold = nullptr;
    dN = nullptr;
}

MainWindow::~MainWindow()
{
    if (unfold)
        free(unfold);
    delete ui;
}

void MainWindow::openWikiPage() {
    QDesktopServices::openUrl(QUrl("https://github.com/gilboonet/Deplieur-CPP/wiki"));
}

void MainWindow::openVideosPage() {
    QDesktopServices::openUrl(QUrl("https://www.youtube.com/playlist?list=PLowPymO2T_CcX8-VNbXaWQd8ghH332D4c"));
}

void MainWindow::langH11() { langHSet(11); }
void MainWindow::langH12() { langHSet(12); }
void MainWindow::langH13() { langHSet(13); }
void MainWindow::langH14() { langHSet(14); }
void MainWindow::langH15() { langHSet(15); }
void MainWindow::langH16() { langHSet(16); }
void MainWindow::langH17() { langHSet(17); }
void MainWindow::langH18() { langHSet(18); }
void MainWindow::langH19() { langHSet(19); }
void MainWindow::langH20() { langHSet(20); }

void MainWindow::langHSet(int h) {
    unfold->hLanguettes = h;
    unfold->displayUI();
}

void MainWindow::langMode0() { langModeSet(0); }
void MainWindow::langMode1() { langModeSet(1); }
void MainWindow::langMode2() { langModeSet(2); }

void MainWindow::langModeSet(int mode) {
    unfold->modeLanguettes = mode;
    unfold->init_flaps();
    unfold->displayUI();
}

void MainWindow::pageFormat_A3() { pageFormat(3); }
void MainWindow::pageFormat_A4() { pageFormat(4); }
void MainWindow::pageFormat_A5() { pageFormat(5); }
void MainWindow::pageFormat_Cr1() { pageFormat(11); }
void MainWindow::pageFormat_Cr2() { pageFormat(12); }
void MainWindow::pageFormat_Cr3() { pageFormat(13); }
void MainWindow::pageFormat_Cr4() { pageFormat(14); }

void MainWindow::pageFormat(int n) {
    if (unfold) {
        unfold->pageFormat(n);
    }
}

void MainWindow::basculerOptimiserNums()
{
    if (unfold) {
        unfold->optimiserNums = true;
        unfold->recalculeNums();
        unfold->displayUI();
    }
}

void MainWindow::quitter() {
    QApplication::quit();
}

void MainWindow::nouveau() {
    if (dN)
        delete dN;
    dN = new DialogNouveau();
    connect(dN, &QDialog::finished, this, &MainWindow::lanceNouveau);
    dN->open();
}

void MainWindow::lanceNouveau() {
    auto fileContentReady = [&](const QString &obj, const QByteArray &donnees) {
        if (obj.isEmpty()) {
            // Aucune fichier sélectionné
            return;
        } else {
            RN_rep r = dN->retourneEdite();
            unfold = new Unfold(obj.toStdString(), "", "", vue, &donnees, r.b);
            switch(r.a) {
                case 0 : pageFormat_A3(); break;
                case 1 : pageFormat_A4(); break;
                case 2 : pageFormat_A5(); break;
                case 3 : pageFormat_Cr1(); break;
                case 4 : pageFormat_Cr2(); break;
                case 5 : pageFormat_Cr3(); break;
                case 6 : pageFormat_Cr4(); break;
                default :pageFormat_A4();
            }

            unfold->unfolding();
            unfold->init_flaps();
            unfold->displayUI();
            unlockMenus();
        }
    };

    QFileDialog::getOpenFileContent("Fichier 3d (*.obj)",  fileContentReady);
}

void MainWindow::ouvrir() {
    auto fileContentReady = [&](const QString &dat, const QByteArray &donnees) {
        if (dat.isEmpty()) {
            // Aucun fichier sélectionné
            return;
        } else {
            unfold = new Unfold("", dat.toStdString(), "", vue, &donnees, 1.0);
            switch(unfold->pageId) {
                case 4 : pageFormat_A4(); break;
                case 3 : pageFormat_A3(); break;
                case 5 : pageFormat_A5(); break;
                case 11 : pageFormat_Cr1(); break;
                case 12 : pageFormat_Cr2(); break;
                case 13 : pageFormat_Cr3(); break;
                case 14 : pageFormat_Cr4();
            }

            unlockMenus();
            switch(unfold->modeLanguettes) {
                case 0 : ui->actionLang0->setChecked(true); break;
                case 1 : ui->actionLang1->setChecked(true); break;
                case 2 : ui->actionLang2->setChecked(true);
            }
            unfold->displayUI();
        }
    };
    QFileDialog::getOpenFileContent("Depliage (*.dat)", fileContentReady);

}

void MainWindow::sauver() {
    if (unfold) {
        std::stringstream ss;
        unfold->save_unfold(ss);
        QFileDialog::saveFileContent(QByteArray::fromStdString(ss.str()), "gabarit.dat");
    }
}

void MainWindow::exporterSVG() {
    if (unfold) {
        unfold->displayUI("export2.svg");
        QFileDialog::saveFileContent(unfold->svgRoot, "export.svg");
    }
}

void MainWindow::unlockMenus()
{
    ui->menuAffichage->setEnabled(true);
    ui->menuGabarit->setEnabled(true);
    ui->menuPiece->setEnabled(true);
}

void MainWindow::tournerD() {
    unfold->rotatePieceCourante(-1);
    unfold->displayUI();
}

void MainWindow::tournerG() {
    unfold->rotatePieceCourante(1);
    unfold->displayUI();
}


void MainWindow::tournerD05() {
    unfold->rotatePieceCourante(-5);
    unfold->displayUI();
}

void MainWindow::tournerG05() {
    unfold->rotatePieceCourante(5);
    unfold->displayUI();
}

void MainWindow::tournerD15() {
    unfold->rotatePieceCourante(-15);
    unfold->displayUI();
}

void MainWindow::tournerG15() {
    unfold->rotatePieceCourante(15);
    unfold->displayUI();
}

void MainWindow::monter() {
    unfold->deplacePieceCourante(0, -5);
}

void MainWindow::descendre() {
    unfold->deplacePieceCourante(0, 5);
}

void MainWindow::droite() {
    unfold->deplacePieceCourante(5, 0);
}

void MainWindow::gauche() {
    unfold->deplacePieceCourante(-5, 0);
}

void MainWindow::zoomNormal()
{
    vue->resetTransform();
}

void MainWindow::zoomPlus() {
    vue->scale(1.25, 1.25);
}

void MainWindow::zoomMoins() {
    vue->scale(0.8, 0.8);
}
