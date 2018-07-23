/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  /** @class form
  @ingroup Controls
  @brief A parent window with a sizable border, title bar, control box, system menu, minimize and maximize
  */
  struct form : window_impl < form,
    policy::has_background,
    policy::has_cursor,
    policy::has_icon,
    policy::has_titlebar,
    policy::has_size,
    policy::has_move,
    policy::has_show,
    policy::has_close,
    policy::has_invalidate,
    messages::wm_erasebkgnd,
    messages::wm_destroy,
    messages::wm_create,
    messages::wm_size,
    messages::wm_close,
    messages::wm_showwindow,
    messages::wm_activate,
    messages::wm_paint,
    messages::wm_setcursor,
    messages::wm_notifyformat,
    messages::wm_getminmaxinfo,
    messages::wm_mouse_up,
    messages::wm_mouse_down,
    messages::wm_mouse_leave,
    messages::wm_mouse_wheel
  > {


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

    int run() override {
      message oMsg;
      __super::run();
      auto iRet = oMsg.pump(*this);
      return iRet;
    }

    form() {
      window::_exstyle = WS_EX_OVERLAPPEDWINDOW;
      window::_style = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
    }

  protected:
    template <typename, template <typename> typename...> friend struct window_impl;
    static constexpr TCHAR window_class_name[] = _T("wtf_form");

    template <WNDPROC wp> using window_class_type = window_class<window_class_name, wp>;

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

  /** @class dialog
  @ingroup Controls
  @brief A parent window with a fixed border, title bar and system menu.
  */
  struct dialog : form {
    dialog() {
      window::_exstyle = WS_EX_DLGMODALFRAME;
      window::_style = WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE | WS_CLIPCHILDREN;
    }
  };
  /** @class tool_window
  @ingroup Controls
  @brief A parent window with a sizable border and title bar.
  */
  struct tool_window : form {
    tool_window() {
      window::_exstyle = WS_EX_TOOLWINDOW;
      window::_style = WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
    }
  };

}