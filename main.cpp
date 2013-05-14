#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.addLibraryPath("C:/Users/Daniel/qt/LMP/plugins/phonon_backend");
    a.setApplicationName("LMP");
    MainWindow w;
    w.show();
    
    return a.exec();
}
