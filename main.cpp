#include <QApplication>

#include "mainwindow.h"
#include "stripwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();
}
