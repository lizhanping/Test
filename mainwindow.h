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
    MainWindow(QWidget *parent = nullptr);
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
    //浏览器处理程序
    CefRefPtr<SimpleHandler> handler;
};
#endif // MAINWINDOW_H
