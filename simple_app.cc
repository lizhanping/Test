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

}
