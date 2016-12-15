#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plotter = new Plotter(this);
    ui->scrollArea->setWidget(plotter);

    connect( ui->scrollE,         SIGNAL(valueChanged(double)), plotter, SLOT(EChanged(double))  );
    connect( ui->scrollNoise,     SIGNAL(valueChanged(int)),    plotter, SLOT(noiseChanged(int)) );
    connect( ui->scrollTlim,      SIGNAL(valueChanged(int)),    plotter, SLOT(tlimChanged(int))  );
    connect( ui->btnUpdateSignal, SIGNAL(pressed()),            plotter, SLOT(updateSignal())    );

}

MainWindow::~MainWindow()
{
    delete ui;
}
