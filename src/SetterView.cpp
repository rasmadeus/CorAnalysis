#include "SetterView.h"
#include "ui_SetterView.h"

#include "SetterModel.h"
#include "IntDelegate.h"
SetterView::SetterView(QwtPlot* ownerIn, QwtPlot* ownerOuts, QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SetterView)
{
    ui->setupUi(this);

    model = new SetterModel(ownerIn, ownerOuts, this);

    ui->view->setModel(model);
    ui->view->setItemDelegateForColumn(4, new IntDelegate(ui->view));
    ui->view->resizeColumnsToContents();

    connect(ui->view, SIGNAL(clicked(QModelIndex)), SLOT(wasClicked(QModelIndex)));
}

SetterView::~SetterView()
{
    delete ui;
    delete model;
}

void SetterView::resamples()
{
    model->resamples();
}

#include <QColorDialog>
void SetterView::wasClicked(const QModelIndex& index)
{
    if(index.column() != 4){
        ui->view->closePersistentEditor(index);
    }
    else{
        ui->view->edit(index);
    }

    if(index.column() == 0 || index.column() == 1|| index.column() == 2){
        model->setData(index, QVariant(), Qt::EditRole);
    }
    else{
        if(index.column() == 3){
            QColor color = QColorDialog::getColor(
                model->data(index, Qt::BackgroundRole).value<QColor>(),
                this,
                tr("Set color")
            );
            if(color.isValid()){
                model->setData(index, color, Qt::EditRole);
            }
        }
    }
}
