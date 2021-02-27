// Copyright (c) 2013 The Chromium Embedded Framework Authors. All rights
// reserved. Use of this source code is governed by a BSD-style license that
// can be found in the LICENSE file.

#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "include/wrapper/cef_helpers.h"
#include "utils.h"
#include "global.h"

#include<QDebug>
#include <list>
#include<QObject>

class CustomResponseFilter:public CefResponseFilter
{
public:
    CustomResponseFilter(CefRefPtr<CefBrowser> browser):browser_(browser){}
    virtual bool InitFilter() override
    {
        return true;
    }

    virtual FilterStatus Filter(void* data_in,
                                size_t data_in_size,
                                size_t& data_in_read,
                                void* data_out,
                                size_t data_out_size,
                                size_t& data_out_written) override
    {
        DCHECK((data_in_size == 0U && !data_in) || (data_in_size > 0U && data_in));
        DCHECK_EQ(data_in_read, 0U);
        DCHECK(data_out);
        DCHECK_GT(data_out_size, 0U);
        DCHECK_EQ(data_out_written, 0U);

        // All data will be read.
        data_in_read = data_in_size;
        qDebug()<<data_in_size;
        //char* bytes=new char(data_in_read);
        auto bytes=static_cast<char*>(data_in);
        QString str(bytes);
        //获取第一个{开头的地方
        int ls=str.indexOf('{');
        int re=str.lastIndexOf('}');
        QString newStr=str.mid(ls,re-ls+1);
        //解析str
        QJsonObject obj=Utils::DeserializeToObejct(newStr);
        auto code=obj["code"].toInt();
        if(code==1)
        {
            auto data=obj["data"].toObject();
            url=data["exam_url"].toString();
            qDebug()<<"filter url:"<<url;
            lockscreen=data["exam_lock"].toString()=="1";
            examname=data["exam_name"].toString();
            exitpwd=data["exam_lock_password"].toString();
            showtop=data["exam_ui_top"].toString()=="1";
            showbottom=data["exam_ui_bottom"].toString()=="1";
            lock_start_key=data["exam_lock_on_key"].toString();
            lock_end_key=data["exam_lock_off_key"].toString();
            if(data.contains("exam_finish_key"))
            {
                exam_finish_key=obj["exam_lock_off_key"].toString();
            }
            else
            {
                exam_finish_key=lock_end_key;
            }
        }
        return FilterStatus::RESPONSE_FILTER_DONE;
    }

private:
    CefRefPtr<CefBrowser> browser_;
  IMPLEMENT_REFCOUNTING(CustomResponseFilter);
};


class SimpleHandler : public QObject,
                      public CefClient,
                      public CefContextMenuHandler,
                      public CefDisplayHandler,
                      public CefLifeSpanHandler,
                      public CefDragHandler,
                      public CefLoadHandler,
                      public CefRequestHandler,
                      public CefResourceRequestHandler
{
    Q_OBJECT
public:
   SimpleHandler();
  ~SimpleHandler() override;

  // Provide access to the single global instance of this object.
  static SimpleHandler* GetInstance();

  // CefClient methods:
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
    return this;
  }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE {
      return this;
  }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE{
      return this;
  }


  // CefDisplayHandler methods:
  virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                             const CefString& title) OVERRIDE;

  virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& url) override;

  // CefLifeSpanHandler methods:
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

  //CefContextMenuHandler methods
  virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() override{
      return this;
  }

  virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) override
  {
      if((params->GetTypeFlags()&(CM_TYPEFLAG_PAGE|CM_TYPEFLAG_FRAME))!=0)
      {
          if(model->GetCount()>0)
          {
              model->Clear();
          }
      }
  }


  // CefLoadHandler methods:
  virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           ErrorCode errorCode,
                           const CefString& errorText,
                           const CefString& failedUrl) OVERRIDE;

  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           TransitionType transition_type) override;

  //CefResourceRequestHandler methods:
  virtual CefRefPtr<CefResponseFilter> GetResourceResponseFilter(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      CefRefPtr<CefResponse> response) override
  {
      //qDebug()<<"get resource:"<<QString::fromStdString(request->GetURL().ToString());
      //返回特殊构造的resopnsefilter
      return new CustomResponseFilter(browser);
      //return nullptr;
  }

    //CefRequestHandler methods:
  virtual CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
      CefRefPtr<CefBrowser> browser,
      CefRefPtr<CefFrame> frame,
      CefRefPtr<CefRequest> request,
      bool is_navigation,
      bool is_download,
      const CefString& request_initiator,
      bool& disable_default_handling) {

     // qDebug()<<QString::fromStdString(request->GetURL().ToString());
      if(QString::fromStdString(request->GetURL().ToString()).contains("http://www.safeexamclient.com/login/exam"))
      {
          return this;
      }
      return nullptr;
  }


  // Request that all existing browser windows close.
  void CloseAllBrowsers(bool force_close);

  bool IsClosing() const { return is_closing_; }

  void NotifyUrlChanged(const QString& url);

 private:
  // Platform-specific implementation.
  void PlatformTitleChange(CefRefPtr<CefBrowser> browser,
                           const CefString& title);

  // List of existing browser windows. Only accessed on the CEF UI thread.
  typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
  BrowserList browser_list_;
  bool is_closing_;
  bool is_load_error;
  CefString failUrl;

public:
signals:
  void urlChanged(const QString& url);
  void afterCreated();
  void loadStart(const QString& url);

 public:
  CefRefPtr<CefBrowser> getCurrentBrowser()
  {
      if(browser_list_.size()!=0)
        return browser_list_.front();
      return nullptr;
  }

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(SimpleHandler);
};



#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
