#include "exitwindow.h"

#include<QVBoxLayout>
#include<QPushButton>
#include<QLineEdit>
#include<QWidget>

ExitWindow::ExitWindow(QString pwd,QWidget* parent):QDialog(parent),m_pwd(pwd)
{
    this->InitComponent();
}

void ExitWindow::InitComponent()
{
    this->setFixedSize(250,120);
    this->setWindowTitle(QStringLiteral("请输入退出密码："));
    //布局
    QVBoxLayout* mainlayout=new QVBoxLayout(this);

    QLineEdit* line=new QLineEdit();
    line->setFixedSize(150,30);
    line->setContentsMargins(0,0,0,0);

    QPushButton* confirmBtn=new QPushButton(QStringLiteral("确认"));
    confirmBtn->setFixedSize(100,30);

    mainlayout->addStretch();
    mainlayout->addWidget(line,0,Qt::AlignCenter);
    mainlayout->addSpacing(20);
    mainlayout->addWidget(confirmBtn,0,Qt::AlignCenter);
    mainlayout->addStretch();

    //绑定事件
    connect(confirmBtn,&QPushButton::clicked,[this,line](){
       if(line->text().trimmed().isEmpty())
           return;
       QString val=line->text().trimmed();
       if(val==m_pwd)
       {
           this->done(1);
       }
       else
       {
           this->done(0);
       }
    });

}

