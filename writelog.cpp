#include "writelog.h"
#include<QTextStream>
#include<QDateTime>
#include<QMutex>
#include<QDir>
#include<QCoreApplication>
#include"qapplication.h"

writelog::writelog()
{

}
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    case QtInfoMsg:
        text = QString("Info:");
        break;
    default:        
        break;
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString short_date = QDateTime::currentDateTime().toString("yyyy-MM-dd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);
    QString logpath=QApplication::applicationDirPath()+"/log";
    auto logfilename=logpath+"/log_"+short_date+".txt";
    QDir dir;

    if(dir.exists(logpath))
    {
      QFile file(logfilename);
      file.open(QIODevice::WriteOnly | QIODevice::Append);
      QTextStream text_stream(&file);
      text_stream << message << "\r\n";
      file.flush();
      file.close();
      mutex.unlock();
    }
    else
    {
       bool ok = dir.mkdir(logpath);//只创建一级子目录，即必须保证上级目录存在
       if(ok)
       {
           QFile file(logfilename);
           file.open(QIODevice::WriteOnly | QIODevice::Append);
           QTextStream text_stream(&file);
           text_stream << message << "\r\n";
           file.flush();
           file.close();
           mutex.unlock();
       }
       else
       {
            mutex.unlock();
           return ;
       }
    }
}



