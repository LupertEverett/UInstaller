#include "UInstaller.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UInstaller w;
    w.show();
    return a.exec();
}
