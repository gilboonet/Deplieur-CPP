#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include "unfold.h"

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
    void handleQuitterTriggered();
    void handleOuvrirTriggered();

    void hSliderValueChanged(int value);
    void zoomNormal();
    void zoomPlus();
    void zoomMoins();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *Scene;
};
#endif // MAINWINDOW_H
