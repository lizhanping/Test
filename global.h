#ifndef GLOBAL_H
#define GLOBAL_H
#include<QString>
#include<QPixmap>

extern bool lockscreen; //是否锁屏
extern bool envcheck; //是否环境检查

extern bool showtop; //显示top
extern bool showlogo;//显示log logshow=showtop&showlogo
extern bool showtime;//显示时间
extern QString logoname;//logo数据
extern QString examname;//考试项目名称
extern bool showname;//显示名称

extern bool showbottom; //显示底部
extern bool showexitbtn;//显示底部退出按钮
extern bool showlocalip;//显示本地ip
extern QString exitpwd;//退出密码

extern QString url;//初始加载url

extern QString lock_start_key; //锁屏启动关键字
extern QString lock_end_key; //锁屏结束关键字

extern bool islocking;//锁定状态

#endif // GLOBAL_H
