#include "editablelistview.h"
#include "editablelistmodel.h"

EditableListView::EditableListView(QWidget *parent) :
    QListView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint const&)),
                     this, SLOT(onCustomContextMenuRequested(QPoint const&)));
}

//QSize EditableListView::viewportSizeHint() const
//{
//    return QSize(256,128);
//}

void EditableListView::onCustomContextMenuRequested(QPoint const& pos)
{
    QModelIndex index = this->currentIndex();
    EditableListModel* data_model = dynamic_cast<EditableListModel*>(this->model());
    if(data_model != nullptr)
    {
        data_model->doContextMenu(this, index, viewport()->mapToGlobal(pos));
    }
}
