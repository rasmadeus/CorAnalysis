#ifndef INTDELEGATE_H
#define INTDELEGATE_H

#include <QItemDelegate>

class IntDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit IntDelegate(QWidget* parent = 0);
protected:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
};

#endif // INTDELEGATE_H
