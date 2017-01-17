#include "editablelistview.h"
#include "editablelistmodel.h"

tf2mc::EditableListView::EditableListView(QWidget *parent) :
    QListView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint const&)),
                     this, SLOT(onCustomContextMenuRequested(QPoint const&)));
}

//QSize tf2mc::EditableListView::viewportSizeHint() const
//{
//    return QSize(256,128);
//}

void tf2mc::EditableListView::onCustomContextMenuRequested(QPoint const& pos)
{
    QModelIndex index = this->currentIndex();
    EditableListModel* data_model = dynamic_cast<EditableListModel*>(this->model());
    if(data_model != nullptr)
    {
        data_model->doContextMenu(this, index, viewport()->mapToGlobal(pos));
    }
}
