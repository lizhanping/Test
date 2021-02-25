#ifndef CALLBACKHANDLER_H
#define CALLBACKHANDLER_H

#include "simple_handler.h"
#include<QString>
#include<QDebug>
#include "mainwindow.h"


class CallBackHandler: public SimpleHandler::Delegate
{
public:
    CallBackHandler(MainWindow* window);
    virtual void UrlChanged(const QString& url) override;
    virtual void UpdateForm() override;
private:
    MainWindow* mainwindow_;
};

#endif // CALLBACKHANDLER_H
