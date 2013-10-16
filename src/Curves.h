#ifndef CURVES_H
#define CURVES_H

class File;
#include <QHash>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>

class Curves
{
public:
    static Curves* getInstance(QwtPlot* ownerIn = NULL, QwtPlot* ownerOuts = NULL);
    ~Curves();
    bool isX(int i) const;
    bool isY(int i) const;
    bool isIn(int i) const;
    void setX(int i);
    void setY(int i);
    void setIn(int i);
    void setPenColor(int i, const QColor& color);
    QColor penColor(int i) const;
    void setPenWidth(int i, int width);
    int penWidth(int i) const;
    void resamples();
    int getInputIndex() const;
    int getArgIndex() const;
    QList<int> getOutputsIndexes() const;
    void setTitleX();
private:
    Curves(QwtPlot* ownerIn, QwtPlot* ownerOuts);
    Curves(const Curves&);
    Curves& operator = (const Curves&);
    static Curves* instance;
    QwtPlotCurve* createCurve(int i) const;
    void insertNewCurve(int i);
    File* file;
    QwtPlot* ownerIn;
    QwtPlot* ownerOuts;
    int iX;
    int iIn;
    QwtPlotCurve* in;
    QHash<int, QwtPlotCurve*> curves;
};

#endif // CURVES_H
