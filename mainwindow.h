/*
 * mainwindow.h
 *
 * In this header file the MainWindow class is declared, which defines the GUI of the application.
 * This file connects user interactions to the Collatz computation logic and
 * manages communication with the worker thread using Qt signals and slots.
 *
 * Built with C++ in Qt Creator using MSVC 2022.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "collatzcalculator.h"
#include<QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CollatzCalculator* obj = nullptr;
    QThread * thObj = nullptr;
public slots:
    void onStart();
    void onStop();
    void onExit();
    void onOverFlow();
    void onResultsComputed(int bestNumber, int bestChainLength, long long duration);
    void updateStartButton();
};
#endif // MAINWINDOW_H
