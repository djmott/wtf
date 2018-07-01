/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  namespace policy {
    template <typename _impl_t>
    struct form_impl : window_impl < _impl_t,
      policy::has_background,
      policy::has_cursor,
      policy::has_icon,
      policy::has_titlebar,
      policy::has_size,
      policy::has_move,
      policy::has_show,
      policy::has_close,
      policy::has_invalidate,
      messages::wm_destroy,
      messages::wm_create,
      messages::wm_size,
      messages::wm_close,
      messages::wm_showwindow,
      messages::wm_activate,
      messages::wm_paint,
      messages::wm_setcursor,
      messages::wm_notifyformat,
      messages::wm_getminmaxinfo
    >{

      static constexpr TCHAR window_class_name[] = _T("wtf_form"); 

      template <WNDPROC wp> using window_class_type = window_class<window_class_name, wp>;

      int top() const override {
        return rect<coord_frame::screen>::get(*this).top;
      }

      int left() const override {
        return rect<coord_frame::screen>::get(*this).left;
      }

      int width() const override {
        auto r = rect<coord_frame::client>::get(*this);
        return r.right - r.left;
      }

      int height() const override {
        auto r = rect<coord_frame::client>::get(*this);
        return r.bottom - r.top;
      }

      void close() noexcept override { ::DestroyWindow(*this); }

      int run() override final {
        message oMsg;
        __super::run();
        auto iRet = oMsg.pump(*this);
        return iRet;
      }

    protected:

      void on_wm_destroy() override {
        bool bQuit = false;
        {
          std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
          auto & oForms = wtf::_::_active_forms();
          auto it = std::find(oForms.begin(), oForms.end(), this);
          if (oForms.end() != it) {
            oForms.erase(it);
            bQuit = (0 == oForms.size());
          }
        }
        if (bQuit) ::PostQuitMessage(0);
        __super::on_wm_destroy();
        __super::_handle = nullptr;
      }

      void on_wm_create() override {
        std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
        wtf::_::_active_forms().push_back(this);
        __super::on_wm_create();
      }

    };
  }



  /** @class form
  @ingroup Controls
  @brief A parent window with a sizable border, title bar, control box, system menu, minimize and maximize
  */
  struct form : policy::form_impl <form> {
    static constexpr DWORD ExStyle = WS_EX_OVERLAPPEDWINDOW;
    static constexpr DWORD Style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
  };

  /** @class dialog
  @ingroup Controls
  @brief A parent window with a fixed border, title bar and system menu.
  */
  struct dialog : policy::form_impl < dialog> {
    static constexpr DWORD ExStyle = WS_EX_DLGMODALFRAME;
    static constexpr DWORD Style = WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE;
  };
  /** @class tool_window
  @ingroup Controls
  @brief A parent window with a sizable border and title bar.
  */
  struct tool_window : policy::form_impl <tool_window> {
    static constexpr DWORD ExStyle = WS_EX_TOOLWINDOW;
    static constexpr DWORD Style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
  };
  
}