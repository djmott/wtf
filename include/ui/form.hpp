#pragma once

namespace wtf{

  template <typename _Ty, DWORD _ExStyle, DWORD _Style>
  struct form_base : window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show, policy::has_cursor,
    policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint, policy::has_mouse, policy::has_click>
  {

    using _super_t = window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show, policy::has_cursor,
      policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint, policy::has_mouse, policy::has_click>;

    static const DWORD ExStyle = _ExStyle;
    static const DWORD Style = _Style;

    form_base(HWND hParent) : _super_t(hParent){}
    form_base() : form_base(nullptr){}
    form_base(const form_base&) = delete;
    form_base &operator=(const form_base &) = delete;
    form_base(form_base&&) = delete;
    form_base &operator=(form_base&&) = delete;

    int exec(bool show_window = true){
      _QuitOnDestroy = true;
      if (show_window) this->show();
      message oMsg;
      auto iRet = oMsg.pump();
      _QuitOnDestroy = true;
      return iRet;
    }

    callback<void(const point::client_coords& p)> OnResize;
    callback<void(const point::client_coords& p)> OnMoved;

    virtual int top() const override{
      return rect::screen_coords::get(*this).top;
    }

    virtual int left() const  override{
      return rect::screen_coords::get(*this).left;
    }

    virtual int width() const  override{
      auto r = rect::client_coord::get(*this);
      return r.right - r.left;
    }

    virtual int height() const  override{
      auto r = rect::client_coord::get(*this);
      return r.bottom - r.top;
    }


  protected:

    virtual void MovingEvent(rect::screen_coords&){}
    virtual void MovedEvent(const point::client_coords& p) override{ OnMoved(p); }
    virtual void ResizingEvent(rect::screen_coords&){}
    virtual void ResizedEvent(typename _super_t::wm_size_flags, const point::client_coords& p) override{ OnResize(p); }

    bool _QuitOnDestroy = false;

    virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool& bhandled) override{
      if (WM_DESTROY == umsg && _QuitOnDestroy){
        PostQuitMessage(0);
        bhandled = true;
        return 0;
      }
      return 0;
    }
  };


  struct form : form_base<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW>{


    form(HWND hParent=nullptr) : form_base(hParent){}
    form(const form&) = delete;
    form &operator=(const form &) = delete;
    form(form&&) = delete;
    form &operator=(form&&) = delete;

  };


}
