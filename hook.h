﻿#ifndef HOOK_H
#define HOOK_H
#include<QObject>
#ifdef Q_OS_WIN
#include"Windows.h"

class Hook
{

public:
    Hook();
    static void installHook();
    static void unInstallHook();
};

#endif
#endif // HOOK_H
