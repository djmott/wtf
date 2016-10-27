#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_form, _Policies...> 
    : public window_impl<_ImplT, _Policies..., policy::has_titlebar, policy::has_show, policy::has_size, policy::has_move,
    policy::has_cursor, policy::has_icon,  policy::wm_destroy, policy::wm_create, policy::wm_erasebkgnd, policy::wm_size>
  {
    using __super_t = window_impl<_ImplT, _Policies..., policy::has_titlebar, policy::has_show, policy::has_size, policy::has_move,
      policy::has_cursor, policy::has_icon, policy::wm_destroy, policy::wm_create, policy::wm_erasebkgnd,
      policy::wm_size>;
    template <typename, policy ... > friend class window_impl;
  public:

    explicit window(iwindow * pParent) : __super_t(pParent){}

    window() : window(nullptr){}

    virtual void show() override{
      if (!_handle) __super_t::exec();
      __super_t::show();
    }

    virtual int exec() override{
      message oMsg;
      __super_t::exec();
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
    virtual void handle_msg(window_message& msg) override{}

    virtual void on_wm_destroy() override{
      bool bQuit = false;
      {
        std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
        auto & oForms = _::_active_forms();
        auto it = std::find(oForms.begin(), oForms.end(), this);
        if (oForms.end() != it){
          oForms.erase(it);
          bQuit = (0 == oForms.size());
        }
      }
      if (bQuit) ::PostQuitMessage(0);
      __super_t::on_wm_destroy();
    }

    virtual void on_wm_create() override{
      std::unique_lock<std::mutex> oLock(_::_active_forms_lock());
      _::_active_forms().push_back(this);
      __super_t::on_wm_create();
    }

  };

  struct form : window<form, policy::isa_form>{
    static const DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
    static const DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    explicit form(iwindow * pParent) : window(pParent){}
    form() : form(nullptr){}

  };

}
