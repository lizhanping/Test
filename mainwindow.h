#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/cef_app.h"
#include "simple_handler.h"

#include <QWidget>
#include <QIcon>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QString url,QString name,QPixmap icon,QString ip,QString exitpwd,bool showtime,bool lockscreen,QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitComponent();

public:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

//包含三部分
private:
    //主浏览器
    QWidget* mainView;
    //顶层
    QWidget* topWidget;
    //底层状态栏
    QWidget* statusBar;
    //加载的url
    QString m_url;
    //加载的name；
    QString m_name;
    //加载的icon
    QPixmap m_icon;
    //本地ip
    QString m_ip;
    //退出密码
    QString m_exitpwd;
    //显示时间
    bool m_showtime;
    //是否锁屏
    bool m_lockscreen;
    //浏览器处理程序
    CefRefPtr<SimpleHandler> handler;
};
#endif // MAINWINDOW_H
