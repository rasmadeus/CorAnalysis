#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Plot.h"
#include "SetterView.h"
#include "File.h"
#include <Plots.h>
#include "Splitter.h"
#include <QSettings>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    file = File::file(this);

    plots = new Plots(this);
    setCentralWidget(plots);

    setterView = new SetterView(plots->in, plots->outs, ui->filesData);
    ui->filesData->setWidget(setterView);

    connect(ui->actionLoad, SIGNAL(triggered()), SLOT(load()));
    connect(file, SIGNAL(dataWasLoaded()), setterView, SLOT(resamples()));
    connect(file, SIGNAL(dataWasLoaded()), SLOT(setTitle()));
    connect(ui->actionStart, SIGNAL(triggered()), SLOT(go()));

    QSettings s;
    s.beginGroup("MainWindow");
        restoreState(s.value("state").toByteArray());
        restoreGeometry(s.value("geometry").toByteArray());
    s.endGroup();
}

MainWindow::~MainWindow()
{
    QSettings s;
    s.beginGroup("MainWindow");
        s.setValue("state", saveState());
        s.setValue("geometry", saveGeometry());
    s.endGroup();

    delete ui;
    delete file;
}

#include <QFileDialog>
void MainWindow::load()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        tr("Load file"),
        file->getPath(),
        tr("Text files (*.txt);;Any files (*)")
    );

    if(path.isEmpty()){
        return;
    }

    file->load(path);
}

#include "Algorithm.h"
void MainWindow::go()
{
    Algorithm* algorithm = new Algorithm(this);
    connect(algorithm, SIGNAL(wasFinished(Algorithm*)), SLOT(wasFinished(Algorithm*)));
    algorithm->start();
    ui->actionLoad->setEnabled(false);
}

void MainWindow::wasFinished(Algorithm* algorithm)
{
    plots->in->drawInterval(algorithm->getSplitter()->getSamples());
    delete algorithm;
    ui->actionLoad->setEnabled(true);
}

void MainWindow::setTitle()
{
    setWindowTitle(tr("A raw file was loaded from ") + file->getPath());
}
