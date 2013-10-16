#include "IntDelegate.h"

IntDelegate::IntDelegate(QWidget* parent) :
    QItemDelegate(parent)
{
}

#include <QSpinBox>
QWidget* IntDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)
    QSpinBox* delegate = new QSpinBox(parent);
    delegate->setMinimum(1);
    delegate->setMaximum(10);
    return delegate;
}

void IntDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QSpinBox* delegate = static_cast<QSpinBox*>(editor);
    delegate->setValue(index.data().toInt());
    delegate->selectAll();
}
