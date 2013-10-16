#ifndef PLOTS_H
#define PLOTS_H

#include <QWidget>
class Plot;
class InPlot;

namespace Ui {
class Plots;
}

class Plots : public QWidget
{
    Q_OBJECT
    
public:
    explicit Plots(QWidget *parent = 0);
    ~Plots();    
    InPlot* in;
    Plot* outs;
    Plot* currentPlot() const;
private:
    Ui::Plots *ui;
};

#endif // PLOTS_H
