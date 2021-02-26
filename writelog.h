#ifndef WRITELOG_H
#define WRITELOG_H

#include<QFile>
class writelog
{
public:
    writelog();
//    static void outputMessage(QtMsgType type,
//                                const QMessageLogContext &conttext,
//                                const QString &msg);
};


extern void outputMessage(QtMsgType type,
                            const QMessageLogContext &conttext,
                            const QString &msg);
#endif // WRITELOG_H
