#ifndef EDITABLELISTVIEW_H
#define EDITABLELISTVIEW_H
/*
EditableListView
======================================
QListView subclass which provides custom context menu for a EditableListModel. You can't template
QObject classes so this is hardcoded to that class.
*/
#include <QListView>

class EditableListView :
        public QListView
{
    Q_OBJECT
public:
    explicit EditableListView(QWidget *parent = nullptr);
    ~EditableListView() override = default;

protected:
    //QSize viewportSizeHint() const override;

signals:

public slots:
    // On constuction this is connected to the sginal this->customContextMenuRequested
    void onCustomContextMenuRequested(QPoint const& pos);
};

#endif // EDITABLELISTVIEW_H
