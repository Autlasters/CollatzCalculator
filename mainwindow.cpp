#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextBrowser>
#include <thread>

    MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int SliderValue = std::thread::hardware_concurrency();
    ui->ThreadsSlider->setMaximum(SliderValue);
    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(false);
    connect(ui->ThreadsSlider, &QSlider::valueChanged, this,[=](int value){ui->LableSliderValue->setText(QString::number(value));});
    connect(ui->ValuesBox, &QSpinBox::valueChanged, this, &MainWindow::updateStartButton);
    connect(ui->ThreadsSlider, &QSlider::valueChanged, this, &MainWindow::updateStartButton);
    connect(ui->StartButton, &QPushButton::clicked, this, &MainWindow::onStart);
    connect(ui->StopButton, &QPushButton::clicked, this, &MainWindow::onStop);
    connect(ui->ExitButton, &QPushButton::clicked, this, &MainWindow::onExit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStartButton(){
    bool enableStart = (ui->ValuesBox->value() > 0) && (ui->ThreadsSlider->value() > 0);
    ui->StartButton->setEnabled(enableStart);
}

void MainWindow::onStart(){
    int numberSpinBoxValue = ui->ValuesBox->value();
    int threadSliderValue = ui->ThreadsSlider->value();
    ui->DisplayField->clear();
    ui->StartButton->setEnabled(false);
    ui->StopButton->setEnabled(true);
    QApplication::processEvents();
    thObj = new QThread(this);
    obj = new CollatzCalculator(numberSpinBoxValue, threadSliderValue, nullptr);
    obj->moveToThread(thObj);
    connect(thObj, &QThread::started, obj, &CollatzCalculator::start);
    connect(obj, &CollatzCalculator::stopComputations, thObj, &QThread::quit);
    connect(thObj, &QThread::finished, thObj, &QObject::deleteLater);
    connect(obj, &CollatzCalculator::stopComputations, obj, &QObject::deleteLater);
    connect(obj, &CollatzCalculator::overFlowDetected, this, &MainWindow::onOverFlow);
    connect(obj, &CollatzCalculator::resultsComputed, this, &MainWindow::onResultsComputed);
    connect(obj, &CollatzCalculator::stopReguestCalledCheck, this, [this](){ ui->DisplayField->append("Calculations were stopped by the user");
                                                                        ui->StopButton->setEnabled(false);
                                                                        updateStartButton();
    });
    thObj->start();
}

void MainWindow::onStop(){
    if(obj){
        obj->stop();
        ui->StopButton->setEnabled(false);
        updateStartButton();
    }
}

void MainWindow::onOverFlow(){
    ui->DisplayField->append("Overflow is detected, computations are not possible!");
}

void MainWindow::onExit(){
    close();
}

void MainWindow::onResultsComputed(int bestNumber, int bestChainLength, long long duration){
        ui->DisplayField->append("The number with the longest chain: " + QString::number(bestNumber));
        ui->DisplayField->append("It's chain: " + QString::number(bestChainLength));
        ui->DisplayField->append("Time: " + QString::number(duration) + " ms");
        ui->StopButton->setEnabled(false);
        updateStartButton();
}
