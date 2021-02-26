#include "callbackhandler.h"
#include "global.h"
#include "utils.h"
#include<QThread>
#include<QWidget>
#include<QLayout>

CallBackHandler::CallBackHandler(MainWindow* window):mainwindow_(window)
{

}

void CallBackHandler::UpdateForm()
{
    mainwindow_->topWidget->setVisible(showtop);
    mainwindow_->statusBar->setVisible(showbottom);
    mainwindow_->resize(mainwindow_->size().width()-1,mainwindow_->size().height()-1);
    mainwindow_->resize(mainwindow_->size().width()+1,mainwindow_->size().height()+1);
    mainwindow_->showFullScreen();
}

void CallBackHandler::UrlChanged(const QString& url)
{
    //检测url中是否包含特定的关键字
    if(lockscreen)
    {
        QString url_str=url;
        if(!lock_start_key.isNull()&&!lock_start_key.isEmpty()&&url_str.contains(lock_start_key))
        {
            if(!islocking)
            {
                Utils::startLock();
                islocking=true;
            }
        }
        else if(!lock_end_key.isNull()&&!lock_end_key.isEmpty()&&url_str.contains(lock_end_key))
        {
           if(islocking)
           {
               Utils::closeLock();
               islocking=false;
           }
        }
    }
}
