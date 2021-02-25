// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.
#include<QApplication>

#include "simple_app.h"
#include<QDebug>
#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"


SimpleApp::SimpleApp() {}

void SimpleApp::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD()
  qDebug()<<"onContextInit";
}

void SimpleApp::OnBeforeCommandLineProcessing(const CefString &process_type,
                                              CefRefPtr<CefCommandLine> command_line)
{
    command_line->AppendSwitchWithValue("enable-media-stream", "1");  //开启摄像头
    command_line->AppendSwitchWithValue("enable-system-flash", "1");  //flash
    command_line->AppendSwitchWithValue("enable-speech-input", "1");  //语音输入
    command_line->AppendSwitchWithValue("enable-experimental-web-platform-features","1");
    command_line->AppendSwitch("use-fake-ui-for-media-stream");
    command_line->AppendSwitch("disable-web-security");
}
