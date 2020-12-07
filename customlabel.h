#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QObject>

class CustomLabel : public QLabel
{
    Q_OBJECT
public:
    CustomLabel();
    ~CustomLabel();

    void mouseDoubleClickEvent(QMouseEvent* event);
signals:
    void mouseDoubleClicked();
};

#endif // CUSTOMLABEL_H
