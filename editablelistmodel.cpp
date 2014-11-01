#include "editablelistmodel.h"
#include "editablelistview.h"
#include <QMenu>
#include <algorithm>
#include <functional>
#include <memory>

EditableListModel::EditableListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_headerText{},
    m_data{}
{
}

// Read functions
Qt::ItemFlags EditableListModel::flags(QModelIndex const& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}
QVariant EditableListModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid()) // ONLY checks that row and column are not negative...
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if(isLastRow(index))
            return QVariant();
        else if (isDataRow(index))
            return QVariant(m_data.at(index.row()));
    }
    return QVariant();
}
QVariant EditableListModel::headerData(int /*section*/, Qt::Orientation /*orientation*/, int /*role*/) const
{
    return QVariant();
}
int EditableListModel::rowCount(QModelIndex const& /*parent*/) const
{
    return static_cast<int>(m_data.size() + 1);
}

// Write functions
// This is used to edit the data structure with or without a delegate installed.
bool EditableListModel::setData (QModelIndex const& index, QVariant const& value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        QString stringValue = value.toString();
        // an existing item
        if(isDataRow(index))
        {
            if (
                // value contains data
                !stringValue.isEmpty()
                // and doesn't contain a value that exists
                && std::find(m_data.begin(), m_data.end(), stringValue) == m_data.end()
                )
            {
                m_data.at(index.row()) = stringValue;
                emit dataChanged(index, index);
                return true;
            }
            else
            {
                // remove this item
                m_data.erase(m_data.begin() + index.row());
                emit dataChanged(index, index);
                // should something else be happening here?
                return true;
            }
        }
        else if(
                // the empty one at the end
                isLastRow(index)
                // and value contains data
                && (!stringValue.isEmpty()
                // and doesn't contain a value that exists
                && std::find(m_data.begin(), m_data.end(), stringValue) == m_data.end())
                )
        {
            beginInsertRows(QModelIndex(), index.row(), index.row());
            m_data.push_back(value.toString());
            endInsertRows();
            //emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}
bool EditableListModel::setHeaderData ( int /*section*/, Qt::Orientation /*orientation*/, QVariant const& /*value*/, int /*role*/ )
{
    return false;
}


void EditableListModel::setHeaderText(QString const& headerText)
{
    m_headerText = headerText;
}

QString const& EditableListModel::getHeaderText() const
{
    return m_headerText;
}
std::vector<QString> const& EditableListModel::getData() const
{
    return m_data;
}

void EditableListModel::addItem(QString const& item)
{
    if(std::find(m_data.begin(), m_data.end(), item) == m_data.end())
    {
        m_data.push_back(item);
    }
}

// got to know which view this comes from so we can activate an editor in it
void EditableListModel::doContextMenu(EditableListView* view, QModelIndex const& index, QPoint const& pos)
{
    if (!index.isValid())
        return;
    QMenu menu{};

    std::unique_ptr<QAction> contextAction_addItem{ new QAction("Add new item", nullptr)};
    QObject::connect(contextAction_addItem.get(), &QAction::triggered,
                     [view, this](){ this->contextMenu_addItem(view);});
    menu.addAction(contextAction_addItem.release());

    if (isDataRow(index))
    {
        std::unique_ptr<QAction> contextAction_editItem{ new QAction("Edit item", nullptr) };
        QObject::connect(contextAction_editItem.get(), &QAction::triggered,
                         [view, index, this](){ this->contextMenu_editItem(view, index);} );
        menu.addAction(contextAction_editItem.release());

        std::unique_ptr<QAction> contextAction_removeItem{ new QAction("Remove item", nullptr) };
        QObject::connect(contextAction_removeItem.get(), &QAction::triggered,
                         [index, this](){ this->contextMenu_removeItem(index);});
        menu.addAction(contextAction_removeItem.release());

    }

    std::unique_ptr<QAction> contextAction_sort{ new QAction("Sort items", nullptr) };
    QObject::connect(contextAction_sort.get(), &QAction::triggered,
                     [this](){ this->contextMenu_sort();});
    menu.addAction(contextAction_sort.release());

    menu.exec(pos);
}

bool EditableListModel::isDataRow(int row) const
{
    return row >= 0 && row < static_cast<int>(m_data.size());
}
bool EditableListModel::isDataRow(QModelIndex const& index) const
{
    return isDataRow(index.row());
}

bool EditableListModel::isLastRow(int row) const
{
    return row == static_cast<int>(m_data.size());
}
bool EditableListModel::isLastRow(QModelIndex const& index) const
{
    return isLastRow(index.row());
}

// private slots
void EditableListModel::contextMenu_addItem(EditableListView* view)
{
    // select the last element and open its editor
    view->edit(createIndex(m_data.size(), 0, nullptr));
}
void EditableListModel::contextMenu_editItem(EditableListView* view, QModelIndex const& index)
{
    view->edit(index);
}
void EditableListModel::contextMenu_removeItem(QModelIndex const& index)
{
    setData(index, QVariant(), Qt::EditRole);
}
void EditableListModel::contextMenu_sort()
{
    beginResetModel();
    std::sort(m_data.begin(), m_data.end(), std::less<QString>());
    endResetModel();
}
