#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;

    // fichier de la forme ":/Translations/App_xx.qm"
    QString tr_folder=":/Translations";
    QString tr_file="UI1_"+QLocale::system().name();

    if(translator.load(tr_file, tr_folder))
        QApplication::installTranslator(&translator);
    else
    {	// pas trouvé
     qCritical("\"%s\" file not Found. No translation will occur.", qPrintable(tr_folder+"/"+tr_file));
    }

    MainWindow w;
    w.show();
    return a.exec();
}
