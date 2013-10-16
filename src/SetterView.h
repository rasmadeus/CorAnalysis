#ifndef SETTERVIEW_H
#define SETTERVIEW_H

#include <QWidget>
#include <qwt_plot.h>
#include <QModelIndex>
class SetterModel;
class SetterModel;

namespace Ui {
class SetterView;
}

class SetterView : public QWidget
{
    Q_OBJECT    
public:
    explicit SetterView(QwtPlot* ownerIn, QwtPlot* ownerOuts, QWidget* parent = 0);
    ~SetterView();
public slots:
    void resamples();
private slots:
    void wasClicked(const QModelIndex& index);
private:
    Ui::SetterView* ui;
    SetterModel* model;
};

#endif // SETTERVIEW_H
