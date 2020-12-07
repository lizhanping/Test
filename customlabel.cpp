#include "customlabel.h"

CustomLabel::CustomLabel()
{

}

CustomLabel::~CustomLabel()
{

}

void CustomLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit this->mouseDoubleClicked();
}
