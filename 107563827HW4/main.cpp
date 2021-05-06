#include "FinalModelViewer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FinalModelViewer w;
    w.show();
    return a.exec();
}
