#ifndef DRAGSENSITIVELISTWIDGET_H
#define DRAGSENSITIVELISTWIDGET_H

#include <QListWidget>

class DragSensitiveListWidget : public QListWidget
{
public:
    DragSensitiveListWidget(QWidget * parent);

protected:
    void dropEvent(QDropEvent *e);
};

#endif // DRAGSENSITIVELISTWIDGET_H
