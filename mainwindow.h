#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/cef_app.h"
#include "simple_handler.h"

#include <QWidget>
#include <QIcon>
#include<QTimer>


class QLabel;
class QPushButton;
class CustomLabel;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitComponent();
    void InitTimer();

public:
    void closeEvent(QCloseEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;

//包含三部分
public:
    //主浏览器
    QWidget* mainView;
    //顶层
    QWidget* topWidget;
    //底层状态栏
    QWidget* statusBar;
    //浏览器处理程序
    CefRefPtr<SimpleHandler> handler;

    //元素部分 -top
    QLabel* icon;
    QLabel* name;
    QLabel* time;
    QPushButton* exitBtn;

    //元素部分 -bottom
    CustomLabel* tipsLabel;
    QPushButton* closeBtn;
    QPushButton* reloadBtn;
    QLabel* ipLabel;

    //定时器
    QTimer* timer;
};
#endif // MAINWINDOW_H
