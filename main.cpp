#include "mainwindow.h"
#include "utils.h"
#include "Windows.h"
#include "hook.h"
#include "simple_app.h"
#include "global.h"

#include <QApplication>
#include<QFile>
#include<QMessageBox>
#include<QSettings>
#include<QDebug>
#include<QTextCodec>
#include<QDesktopWidget>
#include<QTimer>
#include<QNetworkAccessManager>
#include<QUrl>
#include<QNetworkReply>
#include<QJsonObject>
#include<QJsonDocument>

/*
    version 1.1
    date 2020 01.05
    1.增加读取远程项目的接口
    2.有默认的口令输入界面
    3.参数解析功能
    4.
*/



/*  version 1.0
    date 2020 12.05
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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/Image/Exam.ico"));
    QFile file(QApplication::applicationDirPath()+"/Exam.ini");
    //配置文件存在
    if(file.exists())
    {
        QSettings testSettings(file.fileName(),QSettings::IniFormat);
        testSettings.setIniCodec(QTextCodec::codecForName("UTF-8"));
        QString key=testSettings.value("CONFIG/KEY",QVariant("")).toString();
        //判断是否存在KEY。
        //1.如果存在，则通过接口来获取参数信息，并记录到全局
        //2.如果不存在，则读取本地配置信息，如果未能读取到，则使用默认参数配置
        //未读取到key
        if(key.isEmpty())
        {
            lockscreen=testSettings.value("CONFIG/LOCK",QVariant("ON")).toString()=="ON";
            envcheck=testSettings.value("CONFIG/ENVCHECK",QVariant("ON")).toString()=="ON";
            url=testSettings.value("CONFIG/URL",QVariant("http://www.safeexamclient.com/login")).toString();
            examname=testSettings.value("CONFIG/NAME",QVariant(QStringLiteral("在线考试平台"))).toString();
            exitpwd=testSettings.value("CONFIG/PWD",QVariant("")).toString();
            showtime=testSettings.value("CONFIG/SHOWTIME",QVariant("ON")).toString()=="ON";
            showlocalip=testSettings.value("CONFIG/SHOWLOCALIP",QVariant("ON")).toString()=="ON";
            showtop=testSettings.value("CONFIG/SHOWTOP",QVariant("ON")).toString()=="ON";
            showbottom=testSettings.value("CONFIG/SHOWBOTTOM",QVariant("ON")).toString()=="ON";
            showlogo=testSettings.value("CONFIG/SHOWLOGO",QVariant("ON")).toString()=="ON";
            showexitbtn=testSettings.value("CONFIG/SHOWEXITBTN",QVariant("ON")).toString()=="ON";
            logoname=testSettings.value("CONFIG/LOGO",QVariant("")).toString();
            showname=testSettings.value("CONFIG/SHOWNAME",QVariant("ON")).toString()=="ON";
        }
        else
        {
            //读取到key
            //发送http请求，获取json结果，解析，然后配置参数
            QNetworkAccessManager* _manager=new QNetworkAccessManager();
            QString SERVER_URL=QString("http://www.safeexamclient.com/login/exam/")+key;
            QNetworkReply *reply = _manager->get(QNetworkRequest(QUrl(SERVER_URL)));
            QByteArray responseData;
            QEventLoop eventLoop;
            QObject::connect(_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
            eventLoop.exec();       //block until finish
            responseData = reply->readAll();
            qDebug()<<responseData;
            //response 不为空
            if(!(responseData.isNull()||responseData.isEmpty()))
            {
                //解析json
                QJsonDocument doc=QJsonDocument::fromJson(responseData);
                if(doc.isObject())
                {
                    auto val=doc.object();
                    int code=val["code"].toInt();
                    if(code==1)
                    {
                        auto obj=val["data"].toObject();
                        url=obj["exam_url"].toString();
                        lockscreen=obj["exam_lock"].toString()=="1";
                        examname=obj["exam_name"].toString();
                        showtop=obj["exam_ui_top"].toString()=="1";
                        showbottom=obj["exam_ui_bottom"].toString()=="1";
                        lock_start_key=obj["exam_lock_on_key"].toString();
                        lock_end_key=obj["exam_lock_off_key"].toString();
                        qDebug()<<url;
                    }
                    else
                    {
                        //code !=1,代表参数读取有问题,弹框并退出
                        qDebug()<<"code:"<<code<<"message:"<<val["message"];
                        QMessageBox::information(nullptr,QStringLiteral("错误"),val["message"].toString());
                        return 0;
                    }
                }
                else
                {
                    //json解析错误
                    QMessageBox::information(nullptr,QStringLiteral("错误"),QStringLiteral("接口返回数据错误"));
                    return 0;
                }

            }
            else
            {
                //response为空，先提示，然后使用默认
                auto rst=QMessageBox::question(nullptr,QStringLiteral("提示"),QStringLiteral("通过指定的Key获取项目参数失败，\n将使用默认配置，是否继续？"),QStringLiteral("是"),QStringLiteral("否"),0);
                if(rst==1)
                {
                    qDebug()<<"project para read error,user exit client";
                    return 0;
                }
                else
                {
                    //选择了继续，则按照默认配置继续
                }
            }
        }
    }
    else
    {
        //配置文件不存在，则全部使用默认值
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

    MainWindow w;
   int rst=a.exec();
   CefShutdown();
   return rst;
}
