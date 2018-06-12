/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  
  namespace policy{
    template <typename _super_t>
    struct isa_form : _super_t{

      explicit isa_form(window * pParent)  : _super_t(pParent){}

      isa_form() : isa_form(nullptr){}

      void show()  {
        if (!_super_t::_handle) _super_t::run();
        _super_t::show();
      }

      int top() const {
        return rect<coord_frame::screen>::get(*this).top;
      }

      int left() const {
        return rect<coord_frame::screen>::get(*this).left;
      }

      int width() const {
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      int height() const {
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }

       void close() noexcept  { ::DestroyWindow(*this); }

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
        _super_t::on_wm_destroy();
        _super_t::_handle = nullptr;
      }

      void on_wm_create() override{
        std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
        wtf::_::_active_forms().push_back(this);
        _super_t::on_wm_create();
      }

    };
  }

  namespace _ {
    template <typename form_t> using form_impl_super_t = window_impl<form_t,
      policy::isa_form,
      policy::has_background,
      policy::has_cursor, 
      policy::has_icon, 
      policy::has_titlebar, 
      policy::has_size, 
      policy::has_move,
      policy::has_show, 
      policy::has_close, 
      policy::has_invalidate,
      policy::wm_destroy, 
      policy::wm_create, 
      policy::wm_size,
      policy::wm_close, 
      policy::wm_showwindow, 
      policy::wm_activate, 
      policy::wm_paint,
      policy::wm_erasebkgnd,
      policy::wm_setcursor,
      policy::wm_notifyformat>;
  }

  template <DWORD _ExStyle, DWORD _Style>
  struct form_impl : _::form_impl_super_t<form_impl<_ExStyle, _Style>> 
  {
    static constexpr DWORD ExStyle = _ExStyle;
    static constexpr DWORD Style = _Style;

    explicit form_impl(window *parent)  : _::form_impl_super_t<form_impl<_ExStyle, _Style>> (parent) {}
    form_impl() : _::form_impl_super_t<form_impl<_ExStyle, _Style>> (nullptr){
      coinitialize::get();
      wtf::_::init_common_controls::get();
    }

    int run() override final{
      message oMsg;
      _::form_impl_super_t<form_impl<_ExStyle, _Style>>::run();
      auto iRet = oMsg.pump();
      return iRet;
    }

  };


  struct form : form_impl<WS_EX_OVERLAPPEDWINDOW, WS_VISIBLE|WS_OVERLAPPEDWINDOW> {
  private:
    using _super_t = form_impl<WS_EX_OVERLAPPEDWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW>;
  public:
    explicit form(window * pParent)  : _super_t(pParent) {}
    form() noexcept : form(nullptr) {}

  };

  struct dialog : form_impl<WS_EX_DLGMODALFRAME, WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE > {
  private:
    using _super_t = form_impl<WS_EX_DLGMODALFRAME, WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE >;
  public:
    explicit dialog(window * pParent)  : _super_t(pParent) {}
    dialog() noexcept : dialog(nullptr) {}

  };

  struct tool_window : form_impl<WS_EX_TOOLWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW > {
  private:
    using _super_t = form_impl<WS_EX_TOOLWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW>;
  public:
    explicit tool_window(window * pParent)  : _super_t(pParent) {}
    tool_window() noexcept : tool_window(nullptr) {}

  };

}
