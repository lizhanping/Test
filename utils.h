#ifndef UTILS_H
#define UTILS_H

class QString;
#include<QList>
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

    static Hook* hook;
};

#endif // UTILS_H
