#ifndef SETTERMODEL_H
#define SETTERMODEL_H

#include <qwt_plot_curve.h>
#include <QAbstractTableModel>
class Curves;
class File;

class SetterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SetterModel(QwtPlot* ownerIn, QwtPlot* ownerOuts, QObject* parent = 0);
    ~SetterModel();
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    bool setData(const QModelIndex& index, const QVariant& value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    void resamples();
private slots:
    void setTitleX();
private:
    File* file;
    Curves* curves;
};

#endif // SETTERMODEL_H
