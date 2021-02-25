#ifndef UTILS_H
#define UTILS_H

#include<QString>
#include<QList>
#include<QTimer>
#include<QJsonDocument>
#include<QJsonObject>

#include"hook.h"

class Utils
{
public:
    Utils();

public:
    static QString getIp();
    static void startLock();
    static void closeLock();
    static QList<int> GetProcessIdByName(QString name);
    static void KillProcessByName(QString name);
    static void OpenProcessByName(QString path);
    static QJsonObject DeserializeToObejct(QString jsondata);
    static QString SerilizeObject(QJsonObject object);
};

#endif // UTILS_H
