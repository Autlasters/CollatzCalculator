/*
 * main.cpp
 *
 * This .cpp file includes the basic class to display the GUI of the application
 *
 * Built with C++ in Qt Creator using MSVC 2022.
 */
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
