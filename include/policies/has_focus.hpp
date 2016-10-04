#pragma once

namespace wtf {
  namespace policy {
    /** has_focus
    * Provides members and events related to keyboard/mouse focus
    */
    template<typename _SuperT>
    struct has_focus : _SuperT {
      virtual ~has_focus() = default;
      has_focus() = default;
      has_focus(const has_focus&) = delete;
      has_focus(has_focus&&) = delete;
      has_focus &operator=(const has_focus &) = delete;
      has_focus &operator=(has_focus&&) = delete;

      void set_focus() const { wtf::exception::throw_lasterr_if(::SetFocus(*this), [](HWND h) { return !h; }); }

      bool got_focus() const { return _SuperT::_handle == ::GetFocus(); }

      callback<void()> GotFocusEvent;
      callback<void()> LostFocusEvent;
    protected:
      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool &) override {
        if (WM_SETFOCUS == umsg) GotFocusEvent();
        else if (WM_KILLFOCUS == umsg) LostFocusEvent();
        return 0;
      }
    };
  }
}
