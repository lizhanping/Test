#include "mainwindow.h"
#include "customlabel.h"
#include "utils.h"
#include "exitwindow.h"
#include "global.h"

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QTimer>
#include<QCloseEvent>
#include<QMessageBox>
#include<QApplication>
#include<QDateTime>


MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      mainView(new QWidget()),
      topWidget(new QWidget()),
      statusBar(new QWidget()),
      handler(nullptr)
{
    this->InitComponent();
    this->showFullScreen();
}

MainWindow::~MainWindow()
{
    delete mainView;
    delete topWidget;
    delete statusBar;
}

//界面初始化
void MainWindow::InitComponent()
{
    //分为上中下三部分
    QVBoxLayout* mainLayout=new QVBoxLayout(this);

    //1.top
    topWidget->setFixedHeight(48);
    topWidget->setStyleSheet(".QWidget{background:white;border-bottom:1px solid lightgray}");
    topWidget->setVisible(showtop);
    QHBoxLayout* toplayout=new QHBoxLayout(topWidget);
    //1.1 icon
    QLabel* icon=new QLabel();
    icon->setScaledContents(true);
    icon->setFixedSize(32,32);
    QPixmap logoicon;
    if(!logoname.isEmpty())
    {
        //判断当前目录是否存在logo.png文件
        QFile logo(QApplication::applicationDirPath()+"/"+logoname);
        if(logo.exists())
        {
            logoicon.load(logo.fileName());
        }
    }
    icon->setPixmap(logoicon);
    icon->setVisible(showlogo&&!logoicon.isNull());

    //1.2 name
    QLabel* name=new QLabel();
    name->setText(examname);
    name->setScaledContents(true);
    name->setFont(QFont("Microsoft YaHei",18));
    name->setVisible(showname);

    //1.3 time
    QLabel* time=new QLabel();
    time->setFixedHeight(36);
    time->setScaledContents(true);
    time->setFont(QFont("Microsoft YaHei",12));
    time->setStyleSheet(".QLabel{border:1px solid lightgray}");
    time->setContentsMargins(3,3,3,3);
    time->setVisible(showtime);

    //1.4 exitbtn
    QPushButton* exitBtn=new QPushButton();
    exitBtn->setFixedSize(26,26);
    exitBtn->setVisible(showexitbtn);
    exitBtn->setStyleSheet("QPushButton{border-image:url(:/Image/exit-gray.png)}"
                           "QPushButton:hover{border-image:url(:/Image/exit-light.png)}");

    // add all items to layout
    toplayout->addWidget(icon,0,Qt::AlignVCenter);
    toplayout->addWidget(name,0,Qt::AlignVCenter);
    toplayout->addStretch();
    toplayout->addWidget(time,0,Qt::AlignVCenter);
    toplayout->addWidget(exitBtn,0,Qt::AlignVCenter);
    toplayout->setMargin(5);


    //2.mainview
    CefWindowInfo window_info;
    HWND wnd=reinterpret_cast<HWND>(this->mainView->winId());
    RECT winRect;
    //QRect mainRect=this->mainView->rect();
    QRect mainRect={0,0,0,0};
    winRect.left=mainRect.left();
    winRect.right=mainRect.width();
    winRect.top=mainRect.top();
    winRect.bottom=mainRect.height();
    window_info.SetAsChild(wnd,winRect);
    CefBrowserSettings settings;
    handler = CefRefPtr<SimpleHandler>(new SimpleHandler());
    bool flag=CefBrowserHost::CreateBrowser(window_info, handler, url.toStdString(), settings, nullptr,nullptr);
    if(flag)
    {
        qDebug()<<"create browser success!";
        qInfo("Application start..");
    }
    else
    {
        qDebug()<<"application start failed,exit";
        QApplication::exit(0);
    }

    //3.status bar
    statusBar->setFixedHeight(40);
    statusBar->setStyleSheet(".QWidget{background:white;border-top:1px solid lightgray}");
    statusBar->setVisible(showbottom);
    QHBoxLayout* bottomLayout=new QHBoxLayout(statusBar);
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::gray);

    //3.1 tips
    CustomLabel* tipsLabel=new CustomLabel();
    tipsLabel->setText(QStringLiteral("提示：考试过程将全程锁屏！"));
    tipsLabel->setFont(QFont("Microsoft YaHei",12));
    tipsLabel->setScaledContents(true);
    tipsLabel->setPalette(pa);

    //3.2 buttons

    QString btnStyle=".QPushButton{background:transparent;border:1px solid lightblue;border-radius:13px}"
                     ".QPushButton::hover{background:rgba(64,171,225,1);border:1px solid blue}";
    //3.2.1 close
    QPushButton* closeBtn=new QPushButton();
    closeBtn->setFixedSize(80,26);
    QPixmap p1(":/Image/logout.png");
    closeBtn->setIcon(QIcon(p1));
    closeBtn->setText(QStringLiteral("退出"));
    closeBtn->setVisible(false);
    closeBtn->setStyleSheet(btnStyle);

    //3.2.2 reload
    QPushButton* reloadBtn=new QPushButton();
    reloadBtn->setFixedSize(80,26);
    QPixmap p2(":/Image/reload.png");
    reloadBtn->setIcon(QIcon(p2));
    reloadBtn->setText(QStringLiteral("刷新"));
    reloadBtn->setVisible(false);
    reloadBtn->setStyleSheet(btnStyle);

    //3.3 ip
    QLabel* ipLabel=new QLabel();
    ipLabel->setText("IP:"+Utils::getIp());
    ipLabel->setFont(QFont("Microsoft YaHei",12));
    ipLabel->setScaledContents(true);
    ipLabel->setPalette(pa);
    ipLabel->setVisible(showlocalip);

    //add to layout
    bottomLayout->addSpacing(5);
    bottomLayout->addWidget(tipsLabel);
    bottomLayout->addWidget(closeBtn,0,Qt::AlignCenter);
    bottomLayout->addWidget(reloadBtn,0,Qt::AlignCenter);
    bottomLayout->addStretch();
    bottomLayout->addWidget(ipLabel);
    bottomLayout->addSpacing(5);


    //add to mainLayout
    mainLayout->addWidget(topWidget);
    mainLayout->addWidget(mainView);
    mainLayout->addWidget(statusBar);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->setSpacing(0);


    //事件绑定
    connect(tipsLabel,&CustomLabel::mouseDoubleClicked,[closeBtn,reloadBtn](){
        closeBtn->setVisible(true);
        reloadBtn->setVisible(true);
        QTimer* timer=new QTimer();
        timer->singleShot(3000,[closeBtn,reloadBtn](){
            closeBtn->setVisible(false);
            reloadBtn->setVisible(false);
        });
    });

    //按钮事件绑定
    connect(closeBtn,&QPushButton::clicked,[this](){
        this->close();
    });
    connect(reloadBtn,&QPushButton::clicked,[this](){
        //this->mainView->reload();
        handler->getCurrentBrowser()->Reload();
    });
    connect(exitBtn,&QPushButton::clicked,[this]()
    {
        auto rst=QMessageBox::question(this,QStringLiteral("退出"),QStringLiteral("确定要退出考试系统吗？"),QStringLiteral("是"),QStringLiteral("否"),0);
        if(rst==0)
        {
            qDebug()<<"user exit system by click exitbtn";
            QApplication::exit(0);
            return;
        }

    });

    if(showtime)
    {
        //定义一个定时器，定时更新时间
        QTimer* timer=new QTimer();
        timer->setInterval(1000);
        connect(timer,&QTimer::timeout,[time](){
            QDateTime current_date_time =QDateTime::currentDateTime();
            QString current_date =current_date_time.toString("yyyy-MM-dd hh:mm:ss");
            time->setText(current_date);
        });
        timer->start();
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    //判断密码情况
    if(exitpwd=="")
        event->accept();
    else
    {
        ExitWindow* exitWindow=new ExitWindow(exitpwd,this);
        int rst=exitWindow->exec();
        if(rst==1)
            event->accept();
        else
        {
            QMessageBox::information(this,"Error",QStringLiteral("退出密码错误，请重试！"));
            event->ignore();
        }
    }

}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    //调整cef的位置
    if(handler==nullptr)
        return;
    if(handler->getCurrentBrowser()==nullptr)
        return;
            qDebug()<<"resize";
    CefRefPtr<CefBrowser> browser=handler->getCurrentBrowser();
#ifdef Q_OS_WIN
    QRect rect=this->mainView->rect();
    HWND hwnd=browser->GetHost()->GetWindowHandle();
    ::MoveWindow(hwnd,rect.x(),rect.y(),rect.width(),rect.height(),true);
#endif
    event->accept();
}
