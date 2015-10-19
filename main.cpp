#include "mainwindow.h"
#include "scaliseapplication.h"

int main(int argc, char *argv[])
{
    ScaliseApplication a(argc, argv);
    a.addLibraryPath("C:/Users/Daniel/qt/LMP/plugins/phonon_backend");
    a.setApplicationName("LMP");
    MainWindow w;
    w.show();
    
    return a.exec();
}
