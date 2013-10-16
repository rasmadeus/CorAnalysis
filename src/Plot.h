#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <qwt_plot.h>

class Plot : public QwtPlot
{
    Q_OBJECT
public:
    explicit Plot(QWidget *parent = 0);
protected:
    void mouseDoubleClickEvent(QMouseEvent* evt);
};

class Package;
#include <qwt_plot_curve.h>
class InPlot: public Plot
{
    Q_OBJECT
public:
    explicit InPlot(QWidget* parent = 0);
    ~InPlot();
    void clearInterval();
public slots:
    void drawInterval(const QList<Package>& package);
private:
    QwtPlotCurve* splitter;
    QwtPlotCurve* frequency;
};

#endif // PLOTWIDGET_H
