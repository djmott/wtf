#pragma once

namespace wtf{
  
  template <typename _Ty, DWORD _ExStyle, DWORD _Style>
  struct form_base : window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show,
    policy::has_cursor, policy::has_titlebar, policy::has_size, policy::has_close, messages::wm_paint, 
    policy::has_click, messages::wm_create, policy::has_move, messages::wm_destroy, messages::wm_size,
    policy::has_background, messages::wm_erasebkgnd, messages::wm_mouse_down, messages::wm_mouse_up,
    messages::wm_setfocus, messages::wm_setcursor, messages::wm_geticon>
    
  {

    static const DWORD ExStyle = _ExStyle;
    static const DWORD Style = _Style;

    explicit form_base(window<void,void> * pParent) : window(pParent){}

    form_base() : form_base(nullptr){}

    virtual void show() override{
      if (!_handle) window::exec();
      window::show();
    }


    virtual int exec() override{
      window::exec();
      message oMsg;
      auto iRet = oMsg.pump();
      return iRet;
    }
      

    virtual int top() const override{
      return rect<coord_frame::screen>::get(*this).top;
    }

    virtual int left() const  override{
      return rect<coord_frame::screen>::get(*this).left;
    }

    virtual int width() const  override{
      auto r = rect<coord_frame::client>::get(*this);
      return r.right - r.left;
    }

    virtual int height() const  override{
      auto r = rect<coord_frame::client>::get(*this);
      return r.bottom - r.top;
    }
  

  protected:

    virtual LRESULT on_wm_paint(const device_context& dc, const paint_struct& ps, bool& bHandled) override{
      return window::on_wm_paint(dc, ps, bHandled);
    }


    virtual void on_wm_click(const messages::mouse_msg_param& p) override {
      window::on_wm_click(p);
    }

    virtual LRESULT on_wm_size(const point<coord_frame::client>& p, bool& bHandled) override{
      return window::on_wm_size(p, bHandled);
    }


    virtual LRESULT on_wm_destroy(bool& bHandled) override{
      bool bQuit = false;
      {
        std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
        auto & oForms = _::_active_forms();
        oForms.erase(std::find(oForms.begin(), oForms.end(), this));
        bQuit = (0==oForms.size());
      }
      if (bQuit) ::PostQuitMessage(0);
      return window::on_wm_destroy(bHandled);
    }

    virtual LRESULT on_wm_create(bool& bHandled) override{
      std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
      _::_active_forms().push_back(this);
      return window::on_wm_create(bHandled);
    }

  };


  struct form : form_base<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW|WS_VISIBLE>{

    explicit form(window<void,void> * pParent) : form_base(pParent){}
    form() : form(nullptr){}

  };


}
