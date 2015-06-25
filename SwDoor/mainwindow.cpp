#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotter = new Plotter(this);
    ui->scrollArea->setWidget(plotter);

    connect( ui->_noise,SIGNAL(valueChanged(int)),plotter,SLOT(noiseChanged(int)));
    connect( ui->_tlim,SIGNAL(valueChanged(int)),plotter,SLOT(tlimChanged(int)));
    connect( ui->_E,SIGNAL(valueChanged(double)),plotter,SLOT(EChanged(double)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
