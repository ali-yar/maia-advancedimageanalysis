#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Retinal Vessel Segmentation App - AIA/MAIA @UNICAS - Ali Berrada");
    w.show();

    return a.exec();
}
