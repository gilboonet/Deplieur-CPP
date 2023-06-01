#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lineitem.h"

#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <fstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zoomNormal();

    connect(ui->actionOuvrir, &QAction::triggered, this, &MainWindow::ouvrir);
    connect(ui->actionNouveau, &QAction::triggered, this, &MainWindow::nouveau);
    connect(ui->actionSauver, &QAction::triggered, this, &MainWindow::sauver);
    connect(ui->actionQuitter, &QAction::triggered, this, &MainWindow::quitter);

    connect(ui->hSlider, &QSlider::valueChanged, this, &MainWindow::sliderValueChanged);

    connect(ui->actionZoom_Normal, &QAction::triggered, this, &MainWindow::zoomNormal);
    connect(ui->actionZoom_Plus, &QAction::triggered, this, &MainWindow::zoomPlus);
    connect(ui->actionZoom_Moins, &QAction::triggered, this, &MainWindow::zoomMoins);

    connect(ui->actionTourner_5_Droite, &QAction::triggered, this, &MainWindow::tournerD05);
    connect(ui->actionTourner_5_Gauche, &QAction::triggered, this, &MainWindow::tournerG05);
    connect(ui->actionTourner_15_Droite, &QAction::triggered, this, &MainWindow::tournerD15);
    connect(ui->actionTourner_15_Gauche, &QAction::triggered, this, &MainWindow::tournerG15);

    connect(ui->actionMonter, &QAction::triggered, this, &MainWindow::monter);
    connect(ui->actionDescendre, &QAction::triggered, this, &MainWindow::descendre);
    connect(ui->actionGauche, &QAction::triggered, this, &MainWindow::gauche);
    connect(ui->actionDroite, &QAction::triggered, this, &MainWindow::droite);
}

MainWindow::~MainWindow()
{
    free(unfold);
    delete ui;
}

void MainWindow::quitter()
{
    QApplication::quit();
}

void MainWindow::nouveau()
{
    QString obj = QFileDialog::getOpenFileName(
        this, "Ouvrir fichier OBJ", "", "*.obj");
    if (obj.isNull()) {
        return;
    }

    unfold = new Unfold(obj.toStdString(), "", "", ui->graphicsView, ui->hSlider);
    unfold->unfolding();
    unfold->displayUI();

}

void MainWindow::ouvrir()
{
    QString dat = QFileDialog::getOpenFileName(
        this, "Ouvrir Depliage", "", "*.dat");
    QString obj, svg;

    if (dat.isNull()) {
       return;
    }

    QString ext = dat.chopped(4);
    svg = ext + ".svg";

    QFile fDAT(dat);
    if (!fDAT.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(
            this,
            "Deplieur UI",
            "Erreur de fichier"
        );
        return;
    }
    QByteArray line = fDAT.readLine();
    obj = QString(line).trimmed();
    qInfo() << "DAT : " << dat;
    qInfo() << "OBJ : " << obj;
    qInfo() << "SVG : " << svg;

    unfold = new Unfold(obj.toStdString(), dat.toStdString(), svg.toStdString(), ui->graphicsView, ui->hSlider);
    unfold->load_DAT();
    unfold->displayUI();
}

void MainWindow::sliderValueChanged(int value)
{
    unfold->rotatePieceCourante(value);
}

void MainWindow::tournerD05() {
    unfold->rotatePieceCourante(0, 5);
}

void MainWindow::tournerG05() {
    unfold->rotatePieceCourante(0, -5);
}

void MainWindow::tournerD15() {
    unfold->rotatePieceCourante(0, 15);
}

void MainWindow::tournerG15() {
    unfold->rotatePieceCourante(0, -15);
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
    ui->graphicsView->resetTransform();
}

void MainWindow::zoomPlus() {
    ui->graphicsView->scale(1.25, 1.25);
}

void MainWindow::zoomMoins() {
    ui->graphicsView->scale(0.8, 0.8);
}

void MainWindow::sauver() {
    QString dat = QFileDialog::getSaveFileName(this, "Sauver le dépliage", "", "Depliage (*.dat)");
    if (dat.isNull()) {
        return;
    }

    unfold->displayUI();
    std::ofstream sauveDat(dat.toStdString());
    unfold->display_unfold(sauveDat);
    sauveDat.close();
}
