#include "mainwindow.h"

#include <QApplication>
//#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTranslator translator;
    //translator.load(QLocale(), "UI1"_L1, "_"_L1)
    //    QApplication::installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
