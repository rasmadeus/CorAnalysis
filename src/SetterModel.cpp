#include "SetterModel.h"

#include "Curves.h"
#include "File.h"
SetterModel::SetterModel(QwtPlot* ownerIn, QwtPlot* ownerOuts, QObject* parent) :
    QAbstractTableModel(parent)
{

    file = File::file();
    curves = Curves::getInstance(ownerIn, ownerOuts);
    connect(file, SIGNAL(dataWasLoaded()), SLOT(setTitleX()));
}

SetterModel::~SetterModel()
{
    delete curves;
}

int SetterModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return file->size();
}

int SetterModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 5;
}

static QVariant displayRole(const QModelIndex& index, File* file, Curves* curves)
{
    switch(index.column()){
        case 0 : return file->header(index.row());
        case 1 : return QObject::tr(curves->isIn(index.row()) ? "Yes" : "No");
        case 2 : return QObject::tr(curves->isY(index.row()) ? "Yes" : "No");
        case 3 : return QObject::tr("");
        case 4 : return curves->penWidth(index.row());
    }
    return QVariant();
}

static QVariant backgroundRole(const QModelIndex& index, Curves* curves)
{
    if(index.column() == 3){
        return curves->penColor(index.row());
    }
    return QVariant();
}

static QFont fontRole(const QModelIndex& index, Curves* curves)
{
    bool firstIsRed  = (index.column() == 0) && (curves->isX(index.row()));
    bool secondIsRed = (index.column() == 1) && (curves->isIn(index.row()));
    bool thirdIsRed  = (index.column() == 2) && (curves->isY(index.row()));

    if(firstIsRed || secondIsRed || thirdIsRed){
        static QFont large;
        large.setPointSize(QFont().pointSize() + 4);
        large.setBold(true);
        return large;
    }
    return QFont();
}

static QVariant textColorRole(const QModelIndex& index, Curves* curves)
{
    bool firstIsRed  = (index.column() == 0) && (curves->isX(index.row()));
    bool secondIsRed = (index.column() == 1) && (curves->isIn(index.row()));
    bool thirdIsRed  = (index.column() == 2) && (curves->isY(index.row()));

    if(firstIsRed || secondIsRed || thirdIsRed){
        return QColor(255, 0, 0);
    }

    return QVariant();
}

QVariant SetterModel::data(const QModelIndex& index, int role) const
{
    switch(role){
        case Qt::DisplayRole    : return displayRole(index, file, curves);
        case Qt::BackgroundRole : return backgroundRole(index, curves);
        case Qt::FontRole       : return fontRole(index, curves);
        case Qt::TextColorRole  : return textColorRole(index, curves);
    }
    return QVariant();
}

bool SetterModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(role != Qt::EditRole){
        return false;
    }
    beginResetModel();
        switch(index.column()){
            case 0: curves->setX(index.row());                               break;
            case 1: curves->setIn(index.row());                              break;
            case 2: curves->setY(index.row());                               break;
            case 3: curves->setPenColor(index.row(), value.value<QColor>()); break;
            case 4: curves->setPenWidth(index.row(), value.toInt());         break;
        }
    endResetModel();
    return true;
}

QVariant SetterModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
            case 0: return tr("Absciss");
            case 1: return tr("Input");
            case 2: return tr("Outputs");
            case 3: return tr("Color");
            case 4: return tr("Width");
        }
    }
    return QVariant();
}

Qt::ItemFlags SetterModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void SetterModel::resamples()
{
    beginResetModel();
        curves->resamples();
    endResetModel();
}

void SetterModel::setTitleX()
{
    curves->setTitleX();
}
