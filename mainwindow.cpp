#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lineitem.h"

#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zoomNormal();

    connect(ui->actionQuitter, &QAction::triggered, this, &MainWindow::handleQuitterTriggered);
    connect(ui->actionOuvrir, &QAction::triggered, this, &MainWindow::handleOuvrirTriggered);
    connect(ui->hSlider, &QSlider::valueChanged, this, &MainWindow::hSliderValueChanged);

    connect(ui->actionZoom_Normal, &QAction::triggered, this, &MainWindow::zoomNormal);
    connect(ui->actionZoom_Plus, &QAction::triggered, this, &MainWindow::zoomPlus);
    connect(ui->actionZoom_Moins, &QAction::triggered, this, &MainWindow::zoomMoins);

}

MainWindow::~MainWindow()
{
    free(unfold);
    delete ui;
}

void MainWindow::handleQuitterTriggered()
{
    QApplication::quit();
}

void MainWindow::handleOuvrirTriggered()
{
    QString dat = QFileDialog::getOpenFileName(
        this, "Ouvrir Depliage", "", "*.dat");
    QString obj, svg;

    if (dat.isNull()){
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

void MainWindow::hSliderValueChanged(int value)
{
    unfold->rotatePieceCourante(value);
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
