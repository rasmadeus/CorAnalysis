#include "Plots.h"
#include "ui_Plots.h"

#include "Plot.h"
Plots::Plots(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plots)
{
    ui->setupUi(this);
    in = new InPlot(this);
    outs = new Plot(this);
    ui->inLayout->addWidget(in);
    ui->outLayout->addWidget(outs);
}

Plots::~Plots()
{
    delete ui;
}

Plot* Plots::currentPlot() const
{
    return ui->tabWidget->currentIndex() ? outs : in;
}
