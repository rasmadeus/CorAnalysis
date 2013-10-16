#include "Plot.h"
#include <QApplication>
#include <qwt_plot_canvas.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_zoomer.h>

static void setLookAndFeelFrame(QwtPlot* plot)
{
    plot->setAutoFillBackground(true);
    static QFont small;
    small.setPointSize(8);
    plot->setAxisFont(QwtPlot::xBottom, small);
    plot->setAxisFont(QwtPlot::yLeft, small);
    plot->setMinimumSize(QSize(200, 200));
}

static void createCanvas(QwtPlot* plot)
{
    plot->canvas()->setLineWidth(1);
    plot->canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);
    plot->canvas()->setBorderRadius(10);
    plot->canvas()->setPalette(QPalette(Qt::white));
}

static void createGrid(QwtPlot* plot)
{
    QwtPlotGrid* grid = new QwtPlotGrid();
    grid->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid->attach(plot);
}

static void createLegend(QwtPlot* plot)
{
    plot->insertLegend(new QwtLegend(), QwtPlot::TopLegend);
}

static void createZoomer(QwtPlot* plot)
{
   QwtPlotZoomer* zoomer = new QwtPlotZoomer(plot->canvas());
   zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
}

Plot::Plot(QWidget *parent) :
    QwtPlot(parent)
{
    setAutoReplot(true);
    setAxisAutoScale(yLeft);
    setAxisAutoScale(xBottom);
    setLookAndFeelFrame(this);
    createCanvas(this);
    createGrid(this);
    createLegend(this);
    createZoomer(this);
}

#include <QMouseEvent>
void Plot::mouseDoubleClickEvent(QMouseEvent* evt)
{
    if(evt->button() == Qt::LeftButton){
        bool on = axisAutoScale(xBottom);
        setAxisAutoScale(xBottom);
        setAxisAutoScale(yLeft);
        replot();
        if(!on){
            setAxisAutoScale(xBottom, on);
            setAxisAutoScale(yLeft, on);
        }
    }
}

static QwtPlotCurve* createCurve(QwtPlot* owner, const QString& header, const QColor& color, int z)
{
    QwtPlotCurve* curve = new QwtPlotCurve(header);
    curve->setRenderHint(QwtPlotCurve::RenderAntialiased);
    curve->setPen(QPen(color));
    curve->setZ(z);
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    curve->attach(owner);
    return curve;
}

InPlot::InPlot(QWidget* parent):
    Plot(parent)
{
    splitter = createCurve(this, tr("Amplitudes"), Qt::red, 1);
    frequency = createCurve(this, tr("Frequency"), Qt::green, 2);
}

InPlot::~InPlot()
{
    delete splitter;
}

#include "Splitter.h"
#include <qwt_plot_marker.h>
#include "File.h"
#include "Curves.h"
void InPlot::drawInterval(const QList<Package>& package)
{
    File* file = File::file();
    Curves* curves = Curves::getInstance();
    const int iX = curves->getArgIndex();
    const QVector<double> absciss = file->samples(iX);

    QVector<double> x;
    QVector<double> y;
    QVector<double> fY;

    int isF = 1;
    foreach(Package package, package){
        foreach (Frequency frequency, package.getFrequencies()){
            x  << absciss[frequency.getLeft()]   << absciss[frequency.getRight()];
            y  << isF * frequency.getAmplitude() << isF * frequency.getAmplitude();
            fY << frequency.frequency()          << frequency.frequency();
            isF *= -1;
        }
    }

    splitter->setSamples(x, y);
    frequency->setSamples(x, fY);
}

void InPlot::clearInterval()
{
    splitter->setSamples(QVector<double>(), QVector<double>());
    frequency->setSamples(QVector<double>(), QVector<double>());
    replot();
}
