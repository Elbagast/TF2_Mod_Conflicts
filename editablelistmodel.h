#ifndef EDITABLELISTMODEL_H
#define EDITABLELISTMODEL_H
/*
EditableListModel
======================================
Data model for an editable list of strings with item add, edit, remove and sorting.

Should be used in conjunction with an EditableListView widget if you want to use the custom context menu.
*/
#include <QAbstractListModel>
#include <QString>
#include <vector>

class EditableListView;

class EditableListModel :
        public QAbstractListModel
{
    Q_OBJECT
public:
    explicit EditableListModel(QObject *parent = nullptr);
    ~EditableListModel() override = default;

    // Read functions
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QVariant data(QModelIndex const& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(QModelIndex const& parent = QModelIndex()) const override;

    // Write functions
    bool setData (QModelIndex const& index, QVariant const& value, int role = Qt::EditRole ) override;
    bool setHeaderData ( int section, Qt::Orientation orientation, QVariant const& value, int role = Qt::EditRole ) override;

    // Other access
    void setHeaderText(QString const& headerText);
    QString const& getHeaderText() const;

    std::vector<QString> const& getData() const;

    void addItem(QString const& item);

    void doContextMenu(EditableListView* view, QModelIndex const& index, QPoint const& pos);
signals:

public slots:

private slots:
    void contextMenu_addItem(EditableListView* view);
    void contextMenu_editItem(EditableListView* view, QModelIndex const& index);
    void contextMenu_removeItem(QModelIndex const& index);
    void contextMenu_sort();

private:
    bool isDataRow(int row) const;
    bool isDataRow(QModelIndex const& index) const;
    bool isLastRow(int row) const;
    bool isLastRow(QModelIndex const& index) const;

    QString m_headerText;
    std::vector<QString> m_data;

};

#endif // EDITABLELISTMODEL_H
