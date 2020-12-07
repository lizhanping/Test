#include "utils.h"
#include"Windows.h"
#include "TlHelp32.h"

#include<QString>
#include<QProcess>
#include<QApplication>
#include<QTimer>
#include<QHostAddress>
#include<QNetworkInterface>


Utils::Utils()
{

}


QString Utils::getIp()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}

void Utils::startLock()
{
#ifdef Q_OS_WIN
    auto handler=FindWindow(L"Shell_TrayWnd",NULL);
    ShowWindow(handler,SW_HIDE);
    KillProcessByName("explorer.exe");
    QTimer* timer=new QTimer();
    timer->setInterval(300);
    QObject::connect(timer,&QTimer::timeout,[](){
        KillProcessByName("taskmgr.exe");
        KillProcessByName("Taskmgr.exe");
    });
    timer->start();
#endif
}

void Utils::closeLock()
{
#ifdef Q_OS_WIN
    auto handler=FindWindow(L"Shell_TrayWnd",NULL);
    ShowWindow(handler,SW_SHOW);

    QString explorer;
    UINT size=GetWindowsDirectory(NULL,0);
    wchar_t* path=new wchar_t[size];
    if(GetWindowsDirectory(path,size)!=0)
    {
        explorer=QString::fromWCharArray(path)+"\\";
    }
    explorer+="explorer.exe";
    qDebug()<<explorer;
    OpenProcessByName(explorer);
#endif
}


//get pid by name
QList<int> Utils::GetProcessIdByName(QString name)
{
    QList<int> list;
    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
    PROCESSENTRY32 pInfo;
    pInfo.dwSize=sizeof (pInfo);
    Process32First(hSnapshot,&pInfo);
    do{
        QString processName = (QString::fromUtf16(reinterpret_cast<const unsigned short *>(pInfo.szExeFile)));
        if(processName==name)
        {
            list.append(pInfo.th32ProcessID);
        }

    }while(Process32Next(hSnapshot,&pInfo));
    return list;
}

//kill process by pid
void Utils::KillProcessByName(QString name)
{
    //1.先获取所有pid
    QList<int> pids=GetProcessIdByName(name);
    if(pids.size()==0)
        return;
    foreach (int pid, pids) {
        //2.openprocess
        auto handler=OpenProcess(PROCESS_ALL_ACCESS,0,pid);
        if(handler==NULL)
            return;
        //3.terminateprocess
        TerminateProcess(handler,1);
        CloseHandle(handler);
        qDebug()<<"Terminate process "<<pid<<" success";
    }
}

void Utils::OpenProcessByName(QString path)
{
    QProcess process;
    process.setProgram(path);
    process.startDetached();
}



