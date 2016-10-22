#pragma once

namespace wtf{

  template <typename _Ty, DWORD _ExStyle, DWORD _Style>
  struct form_base : window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show, 
    policy::has_cursor, policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint, 
    policy::has_click, policy::has_create, policy::has_move>
  {

    using _super_t = window<form_base<_Ty, _ExStyle, _Style>, policy::has_icon, policy::has_show,
      policy::has_cursor, policy::has_titlebar, policy::has_size, policy::has_close, policy::has_paint,
      policy::has_click, policy::has_create, policy::has_move>;

    static const DWORD ExStyle = _ExStyle;
    static const DWORD Style = _Style;

    form_base(window<void> * pParent) : _super_t(pParent){
      std::unique_lock<std::mutex> oLock(forms_lock());
      forms().push_back(this);
    }
    form_base() : form_base(nullptr){}

    virtual int exec() override{
      _super_t::exec();
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


    static std::mutex& forms_lock(){
      static std::mutex _forms_lock;
      return _forms_lock;
    }

    static std::vector<form_base*>& forms(){
      static std::vector<form_base*> _forms;
      return _forms;
    }

    LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool& bhandled) {
      if (WM_DESTROY == umsg && _QuitOnDestroy){
        PostQuitMessage(0);
        bhandled = true;
        return 0;
      }
      return 0;
    }
  };


  struct form : form_base<form, WS_EX_OVERLAPPEDWINDOW, WS_OVERLAPPEDWINDOW|WS_VISIBLE>{

    form(window<void> * pParent=nullptr) : form_base(pParent){}
  };


}