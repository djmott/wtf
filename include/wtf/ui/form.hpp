/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT>
    struct isa_form : _SuperT{

      explicit isa_form(window * pParent) : _SuperT(pParent){}

      isa_form() : isa_form(nullptr){}

      void show() override{
        if (!_SuperT::_handle) exec();
        _SuperT::show();
      }

      int top() const override{
        return rect<coord_frame::screen>::get(*this).top;
      }

      int left() const override{
        return rect<coord_frame::screen>::get(*this).left;
      }

      int width() const override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      int height() const override{
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }

       void close() override { ::DestroyWindow(*this); }

    protected:

      void on_wm_destroy() override{
        bool bQuit = false;
        {
          std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
          auto & oForms = wtf::_::_active_forms();
          auto it = std::find(oForms.begin(), oForms.end(), this);
          if (oForms.end() != it){
            oForms.erase(it);
            bQuit = (0 == oForms.size());
          }
        }
        if (bQuit) ::PostQuitMessage(0);
        _SuperT::on_wm_destroy();
        _SuperT::_handle = nullptr;
      }

      void on_wm_create() override{
        std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
        wtf::_::_active_forms().push_back(this);
        _SuperT::on_wm_create();
      }

    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_form>{
      using requires = policy_list<policy::has_background, policy::has_cursor, policy::has_icon,
        policy::has_titlebar, policy::has_size, policy::has_move, policy::has_show, policy::has_close,
        policy::wm_destroy, policy::wm_create, policy::wm_size, policy::wm_close,
        policy::wm_showwindow, policy::wm_activate, policy::wm_paint>;
    };
  }

  template <DWORD _ExStyle, DWORD _Style>
  struct form_impl : window_impl<form_impl<_ExStyle, _Style>, policy::isa_form>{
    static const DWORD ExStyle = _ExStyle;
    static const DWORD Style = _Style;
  private:
    using _SuperT = window_impl<form_impl<_ExStyle, _Style>, policy::isa_form>;
  public:
    explicit form_impl(window * pParent) : _SuperT(pParent){}
    form_impl() : form_impl(nullptr){}

    int run(){
      _::message oMsg;
      _SuperT::exec();
      auto iRet = oMsg.pump();
      return iRet;
    }

  };

  struct form : form_impl<WS_EX_OVERLAPPEDWINDOW, WS_VISIBLE|WS_OVERLAPPEDWINDOW> {
  private:
    using _SuperT = form_impl<WS_EX_OVERLAPPEDWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW>;
  public:
    explicit form(window * pParent) : _SuperT(pParent) {}
    form() : form(nullptr) {}

  };

  struct dialog : form_impl<WS_EX_DLGMODALFRAME, WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE > {
  private:
    using _SuperT = form_impl<WS_EX_DLGMODALFRAME, WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE >;
  public:
    explicit dialog(window * pParent) : _SuperT(pParent) {}
    dialog() : dialog(nullptr) {}

  };

  struct tool_window : form_impl<WS_EX_TOOLWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW > {
  private:
    using _SuperT = form_impl<WS_EX_TOOLWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW>;
  public:
    explicit tool_window(window * pParent) : _SuperT(pParent) {}
    tool_window() : tool_window(nullptr) {}

  };

}
