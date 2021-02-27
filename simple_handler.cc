// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#include "utils.h"
#include "global.h"
#include "mainwindow.h"
#include "simple_handler.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/cef_parser.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/internal/cef_types.h"
#include "include/cef_response_filter.h"

#include <sstream>
#include <string>
#include<QDebug>
#include<QUrl>
#include<QWindow>
#include<QWidget>
#include<QStackedLayout>
#include<QThread>

namespace {

SimpleHandler* g_instance = nullptr;


// Returns a data: URI with the specified contents.
std::string GetDataURI(const std::string& data, const std::string& mime_type) {
  return "data:" + mime_type + ";base64," +
         CefURIEncode(CefBase64Encode(data.data(), data.size()), false)
             .ToString();
}

}  // namespace



SimpleHandler::SimpleHandler():
    is_closing_(false),
    is_load_error(false)
{
    DCHECK(!g_instance);
    g_instance=this;
}

SimpleHandler::~SimpleHandler() {
  g_instance = nullptr;
}

// static
SimpleHandler* SimpleHandler::GetInstance() {
  return g_instance;
}

void SimpleHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title) {
  CEF_REQUIRE_UI_THREAD();
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();

  // Add to the list of existing browsers.
  browser_list_.push_back(browser);
  qDebug()<<"create success";
  emit afterCreated();
  return ;
}

void SimpleHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString &url)
{
    QString url_str=QString::fromStdString(url.ToString());
    emit urlChanged(url_str);
    return;
}


bool SimpleHandler::DoClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  qDebug()<<"do close..";
  // Closing the main window requires special handling. See the DoClose()
  // documentation in the CEF header for a detailed destription of this
  // process.
  if (browser_list_.size() == 1) {
    // Set a flag to indicate that the window close should be allowed.
    is_closing_ = true;
  }

  // Allow the close. For windowed browsers this will result in the OS close
  // event being sent.
  return false;
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
  CEF_REQUIRE_UI_THREAD();
  qDebug()<<"on before close";
  // Remove from the list of existing browsers.
  BrowserList::iterator bit = browser_list_.begin();
  for (; bit != browser_list_.end(); ++bit) {
    if ((*bit)->IsSame(browser)) {
      browser_list_.erase(bit);
      break;
    }
  }

  if (browser_list_.empty()) {
    // All browser windows have closed. Quit the application message loop.
    qDebug()<<"quitmessageloop";
    CefQuitMessageLoop();
  }
}

void SimpleHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl) {
  CEF_REQUIRE_UI_THREAD();

  // Don't display an error for downloaded files.
  if (errorCode == ERR_ABORTED)
    return;

  // Display a load error message using a data: URI.
  std::stringstream ss;
    ss << "<html lang='zh'>"
          "<head>"
          "<meta charset='utf-8'>"
          "<style>"
          "html body{margin:0;padding:0;text-align:center;width:100%;height:100%}"
          ".div1{margin:0 auto;height:400px;position:relative;top:40%}"
          ".refresh{width:100px;height:30px;border:1px solid rgba(0,0,0,0.8);"
          "border-radius:15px;margin:auto;display:block;background-color:#18A2F2;}"
          ".refresh:hover{background-color:#38F2F2;border-:1px solid rgba(255,255,255,0);}"
          ".refresh a{width:100%;height:100%;color:rgba(255,255,255,0.8);display:block;"
          "text-decoration:none;background-color:transparent;font-size:15px;line-height:30px;}"
          "</style>"
          "</head>"
          "<body bgcolor=\"white\">"
          "<div class=\"div1\">"
          "<h4>"
          <<QStringLiteral("提示：考试作答页面加载失败").toStdString()
        <<"</h4>"
          "<div class=\"refresh\">"
          "<a href="<<failedUrl
        <<">"
        <<QStringLiteral("点击刷新").toStdString()
        <<"</a>"
          "</div>"
          "</div>"
          "</body>"
          "</html>";
    frame->LoadURL(GetDataURI(ss.str(), "text/html"));
    is_load_error=true;
    failUrl=failedUrl;
}


void SimpleHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                TransitionType transition_type)
{
    QString url_str=QString::fromStdString(frame->GetURL().ToString());
    emit loadStart(url_str);
    return ;
}


void SimpleHandler::CloseAllBrowsers(bool force_close) {
  if (!CefCurrentlyOn(TID_UI)) {
    // Execute on the UI thread.
    CefPostTask(TID_UI, base::Bind(&SimpleHandler::CloseAllBrowsers, this,
                                   force_close));
    return;
  }

  if (browser_list_.empty())
    return;

  BrowserList::const_iterator it = browser_list_.begin();
  for (; it != browser_list_.end(); ++it)
    (*it)->GetHost()->CloseBrowser(force_close);
}

