#include "global.h"

bool lockscreen=true; //是否锁屏
bool envcheck=false; //是否环境检查

bool showtop=true; //显示top
bool showlogo=true;//显示log logshow=showtop&showlogo
bool showtime=false;//显示时间
QString logoname;//logo数据
QString examname=QStringLiteral("在线考试平台");//考试项目名称
bool showname=true;//显示名称

bool showbottom=true; //显示底部
bool showexitbtn=true;//显示顶部退出按钮
bool showlocalip=true;//显示本地ip
QString exitpwd;//退出密码

QString url=QString("http://www.safeexamclient.com/login");//初始加载url

//需要监听url中是否存在关键字
QString lock_start_key; //锁屏起始
QString lock_end_key; //锁屏结束  //锁屏结束的时候应该显示退出按钮最好

bool islocking=false;//锁定状态
