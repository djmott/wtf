/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  namespace policy {
    template <typename _super_t>
    struct isa_form : _super_t {

      isa_form() = default;

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
        _super_t::on_wm_destroy();
        _super_t::_handle = nullptr;
      }

      void on_wm_create() override {
        std::unique_lock<std::mutex> oLock(wtf::_::_active_forms_lock());
        wtf::_::_active_forms().push_back(this);
        _super_t::on_wm_create();
      }

    };
  }


  template <typename _impl_t,  DWORD _ExStyle, DWORD _Style> struct form_impl : window_impl < form_impl<_impl_t, _ExStyle, _Style>,
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
    policy::wm_notifyformat,
    policy::wm_getminmaxinfo
  > {

    static constexpr DWORD ExStyle = _ExStyle;
    static constexpr DWORD Style = _Style;

    form_impl() = default;

    int run() override final {
      message oMsg;
      __super::run();
      auto iRet = oMsg.pump();
      return iRet;
    }

  protected:


  };

  /** @class form
  @ingroup Widgets
  @brief A parent window with a sizable border, title bar, control box, system menu, minimize and maximize
  */
  struct form : form_impl<form, WS_EX_OVERLAPPEDWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW> {

    form() : form_impl() {}

  };

  /** @class dialog
  @ingroup Widgets
  @brief A parent window with a fixed border, title bar and system menu.
  */
  struct dialog : form_impl<dialog, WS_EX_DLGMODALFRAME, WS_SYSMENU | WS_DLGFRAME | WS_CAPTION | WS_VISIBLE> {

    dialog() : form_impl() {}

  };

  /** @class tool_window
  @ingroup Widgets
  @brief A parent window with a sizable border and title bar.
  */
  struct tool_window : form_impl<tool_window, WS_EX_TOOLWINDOW, WS_VISIBLE | WS_OVERLAPPEDWINDOW> {

    tool_window() : form_impl() {}

  };
  
}