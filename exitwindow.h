#ifndef EXITWINDOW_H
#define EXITWINDOW_H

#include <QDialog>
#include <QObject>

class ExitWindow : public QDialog
{
    Q_OBJECT
public:
    ExitWindow(QString pwd,QWidget* parent=nullptr);
    void InitComponent();


private:
    QString m_pwd;//密码

};

#endif // EXITWINDOW_H
