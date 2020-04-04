#include "mainwindow/takealookmainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TakeALookMainWindow w(nullptr, argv[1]);
    w.show();
    return a.exec();
}
