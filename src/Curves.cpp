#include "Curves.h"

Curves* Curves::instance = NULL;

Curves* Curves::getInstance(QwtPlot* ownerIn, QwtPlot* ownerOuts)
{
    if(instance == NULL){
        instance = new Curves(ownerIn, ownerOuts);
    }
    return instance;
}

#include <QSettings>
#include "File.h"
Curves::Curves(QwtPlot* ownerIn, QwtPlot* ownerOuts):
    ownerIn(ownerIn),
    ownerOuts(ownerOuts),
    iX(0),
    iIn(-1),
    in(NULL)
{
    file = File::file();
    setTitleX();

    QSettings s;
    s.beginGroup("Curves");
        setX(s.value("iX").toInt());
    s.endGroup();
}

Curves::~Curves()
{
    qDeleteAll(curves);

    QSettings s;
    s.beginGroup("Curves");
        s.setValue("iX", iX);
    s.endGroup();
}

bool Curves::isX(int i) const
{
    return i == iX;
}

bool Curves::isY(int i) const
{
    return curves.contains(i);
}

#include "Plot.h"
bool Curves::isIn(int i) const
{
    return i == iIn;
}

void Curves::setX(int i)
{
    if(i == iX){
        return;
    }
    iX = i;
    setTitleX();
    resamples();
    static_cast<InPlot*>(ownerIn)->clearInterval();
}

void Curves::setY(int i)
{
    if(isY(i)){
        delete curves.take(i);
    }
    else{
        insertNewCurve(i);
        curves[i]->setSamples(
            file->samples(iX),
            file->samples(i)
        );
    }

    ownerOuts->replot();
}

void Curves::setIn(int i)
{
    if(i == iIn){
        return;
    }
    iIn = i;
    if(!in){
        in = createCurve(iIn);
        in->setPen(QPen(QColor(50, 50, 50)));
        in->setZ(0);
        in->attach(ownerIn);
    }
    in->setSamples(
        file->samples(iX),
        file->samples(iIn)
    );
    in->setTitle(File::file()->header(iIn));
    static_cast<InPlot*>(ownerIn)->clearInterval();
}

void Curves::setPenColor(int i, const QColor& color)
{
    if(!isY(i)){
        insertNewCurve(i);
    }

    QPen pen = curves[i]->pen();
    pen.setColor(color);
    curves[i]->setPen(pen);

    ownerOuts->replot();
}

QColor Curves::penColor(int i) const
{
    return isY(i) ? curves[i]->pen().color() : Qt::gray;
}

void Curves::setPenWidth(int i, int width)
{
    if(!isY(i)){
        insertNewCurve(i);
    }

    QPen pen = curves[i]->pen();
    pen.setWidth(width);
    curves[i]->setPen(pen);

    ownerOuts->replot();
}

int Curves::penWidth(int i) const
{
    return isY(i) ? curves[i]->pen().width() : 2;
}

void Curves::setTitleX()
{
    QwtText title(file->header(iX));
    title.setFont(QFont());
    ownerOuts->setAxisTitle(QwtPlot::xBottom, title);
    ownerIn->setAxisTitle(QwtPlot::xBottom, title);
}

void Curves::insertNewCurve(int i)
{
    QwtPlotCurve* curve = createCurve(i);
    curves[i] = curve;
    curve->attach(ownerOuts);
}

QwtPlotCurve* Curves::createCurve(int i) const
{
    QwtPlotCurve* curve = new QwtPlotCurve(file->header(i));
    curve->setRenderHint(QwtPlotCurve::RenderAntialiased);
    curve->setPen(QPen(QColor(rand() % 255, rand() % 255, rand() % 255), 2));
    curve->setPaintAttribute(QwtPlotCurve::ClipPolygons);
    return curve;
}

void Curves::resamples()
{
    setTitleX();
    foreach(int i, curves.keys()){
        if(i >= file->size()){
            delete curves.take(i);
        }
        else{
            QwtPlotCurve* curve = curves.value(i);
            curve->setSamples(
                file->samples(iX),
                file->samples(i)
            );
        }
    }

    ownerOuts->repaint();

    if(in){
        if(iIn >= file->size()){
            iIn = -1;
            delete in;
            in = NULL;
        }
        else{
            in->setSamples(
                file->samples(iX),
                file->samples(iIn)
            );
        }
        static_cast<InPlot*>(ownerIn)->clearInterval();
    }
}

int Curves::getInputIndex() const
{
    return iIn;
}

int Curves::getArgIndex() const
{
    return iX;
}

QList<int> Curves::getOutputsIndexes() const
{
    return curves.keys();
}
