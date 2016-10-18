#pragma once

namespace wtf{

  template <typename _Ty, DWORD _ExStyle, DWORD _Style>
  struct form_base : window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show, policy::has_cursor,
    policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint, policy::has_click, policy::has_create>
  {

    using _super_t = window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show, policy::has_cursor,
      policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint, policy::has_click, policy::has_create>;

    static const DWORD ExStyle = _ExStyle;
    static const DWORD Style = _Style;

    form_base(iwindow * pParent) : _super_t(pParent){}
    form_base() : form_base(nullptr){}

    int exec(bool show_window = true){
      _QuitOnDestroy = true;
      make_window();
      if (show_window) this->show();
      message oMsg;
      auto iRet = oMsg.pump();
      _QuitOnDestroy = true;
      return iRet;
    }

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


    bool _QuitOnDestroy = false;

    LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool& bhandled) {
      if (WM_DESTROY == umsg && _QuitOnDestroy){
        PostQuitMessage(0);
        bhandled = true;
        return 0;
      }
      return 0;
    }
  };


  struct form : form_base<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW>{

    form(iwindow * pParent=nullptr) : form_base(pParent){}
  };


}
