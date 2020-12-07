#include "mainwindow.h"
#include "utils.h"
#include "Windows.h"
#include "hook.h"
#include "simple_app.h"

#include <QApplication>
#include<QFile>
#include<QMessageBox>
#include<QSettings>
#include<QDebug>
#include<QTextCodec>
#include<QDesktopWidget>
#include<QTimer>

/*
    本软件用于替代以前的小象软件，实现以下功能：
    1.读取配置文件
    2.依据配置文件设置锁屏、加载设置的url、加载考试名称、icon等
    3.依据配置文件 确定是否环境监测
    4.生成单一的exe+ini文件格式

    设计思路：
    1.使用webengine加载网页
    2.读取本机ip显示，有个状态栏，双击时能够退出
    3.可扩展？
*/

bool isLocking=false;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Image/Test.ico"));
    QFile file(QApplication::applicationDirPath()+"/Test.ini");

    QSettings testSettings(file.fileName(),QSettings::IniFormat);
    testSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    bool lockscreen=testSettings.value("CONFIG/LOCK",QVariant("YES")).toString()=="YES";
    QString url=testSettings.value("CONFIG/URL",QVariant("https://plus.weicewang.com/login")).toString();
    QString name=testSettings.value("CONFIG/NAME",QVariant(QStringLiteral("在线考试平台"))).toString();
    QString pwd=testSettings.value("CONFIG/EXITPWD",QVariant("")).toString();
    bool showtime=testSettings.value("CONFIG/SHOWTIME",QVariant("NO")).toString()=="YES";
    bool staysOnTop=testSettings.value("CONFIG/STAYSONTOP",QVariant("YES")).toString()=="YES";
    QString ip=Utils::getIp();
    QPixmap icon;
    //判断当前目录是否存在logo.png文件
    QFile logo(QApplication::applicationDirPath()+"/logo.png");
    if(logo.exists())
    {
        icon.load(logo.fileName());
    }

    //cef 相关初始化
    CefEnableHighDPISupport();
    HINSTANCE hInstance = static_cast<HINSTANCE>(GetModuleHandle(nullptr));
    CefMainArgs mainArgs(hInstance);
    CefRefPtr<SimpleApp> app(new SimpleApp); //CefApp实现，用于处理进程相关的回调。

    int exit_code = CefExecuteProcess(mainArgs, app, nullptr);
    qDebug()<<"exit_code:"<<exit_code;
    if (exit_code >= 0) {
        return exit_code;
    }

    CefSettings settings;
    settings.multi_threaded_message_loop=true;
    CefInitialize(mainArgs, settings, app.get(), nullptr);

    MainWindow w(url,name,icon,ip,pwd,showtime,lockscreen);
    Hook hook;
    if(lockscreen)
    {
        w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        if(staysOnTop)
            w.setWindowFlags(w.windowFlags()|Qt::WindowStaysOnTopHint);
        Utils::startLock();
        hook.installHook();
        qDebug()<<"start lock";
        isLocking=true;
        w.activateWindow();
        w.raise();
        w.setGeometry(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
        w.show();
        //w.showFullScreen();
        //定义一个定时器，确保resize触发
        QTimer* timer=new QTimer();
        timer->singleShot(2000,[&w](){
           w.resize(w.size()-QSize(1,1));
           w.resize(w.size()+QSize(1,1));
        });

    }
    else
        w.showMaximized();

   int rst=a.exec();
   CefShutdown();
   if(isLocking)
   {
       Utils::closeLock();
       hook.unInstallHook();
       qDebug()<<"close lock";
       isLocking=false;
   }
   return rst;
}
