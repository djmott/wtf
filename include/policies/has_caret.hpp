#pragma once

namespace wtf {
  namespace policy {
    /** has_caret
    * Controls the caret of text/input elements
    */
    template<typename _SuperT>
    struct has_caret : _SuperT {
      has_caret() = default;
      has_caret(const has_caret&) = delete;
      has_caret(has_caret&&) = delete;
      has_caret &operator=(const has_caret &) = delete;
      has_caret &operator=(has_caret&&) = delete;

      void create_caret() const {
        wtf::exception::throw_lasterr_if(::CreateCaret(*this, nullptr, 1, 15), [](BOOL b) { return !b; });
      }

      void destroy_caret() const {
        wtf::exception::throw_lasterr_if(::DestroyCaret(), [](BOOL b) { return !b; });
      }

      void show_caret() const {
        wtf::exception::throw_lasterr_if(::ShowCaret(*this), [](BOOL b) { return !b; });
      }

      void hide_caret() const {
        wtf::exception::throw_lasterr_if(::HideCaret(*this), [](BOOL b) { return !b; });
      }

      UINT caret_blink_time() const {
        return wtf::exception::throw_lasterr_if(::GetCaretBlinkTime(), [](UINT i) { return !i; });
      }

      void caret_blink_time(UINT newval) const {
        wtf::exception::throw_lasterr_if(::SetCaretBlinkTime(newval), [](BOOL b) { return !b; });
      }

      void set_caret_pos(const point &pos) const {
        wtf::exception::throw_lasterr_if(::SetCaretPos(pos.x, pos.y), [](BOOL b) { return !b; });
      }

    protected:
      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool &) override {
        if (WM_SETFOCUS == umsg) {
          std::cout << "make caret" << std::endl;
          create_caret();
          set_caret_pos(point(1, 1));
          show_caret();
          caret_blink_time(250);
        } else if (WM_KILLFOCUS == umsg) {
          std::cout << "destroy caret" << std::endl;
          destroy_caret();
        }
        return 0;
      }
    };
  }
}
