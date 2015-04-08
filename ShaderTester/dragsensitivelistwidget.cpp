#include "dragsensitivelistwidget.h"

DragSensitiveListWidget::DragSensitiveListWidget(QWidget * parent) :
    QListWidget(parent)
{

}

void DragSensitiveListWidget::dropEvent(QDropEvent *e) {
    QListView::dropEvent(e);
}

