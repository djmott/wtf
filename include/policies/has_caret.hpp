#pragma once

namespace wtf {
  namespace policy {
    /** has_caret
    * Controls the caret of text/input elements
    */
    template<typename _SuperT, typename _ImplT>
    struct has_caret : _SuperT {
      has_caret(const has_caret&) = delete;
      has_caret(has_caret&&) = delete;
      has_caret &operator=(const has_caret &) = delete;
      has_caret &operator=(has_caret&&) = delete;
      virtual ~has_caret() = default;
      has_caret() 
        : _SuperT(),
        _blink_rate(250),
        _pos(1,1),
        _width(1),
        _height(1)
      {}

      virtual int caret_width() const{ return _width; }
      virtual void caret_width(int newval){ _width = newval; }

      virtual int caret_height() const{ return _height; }
      virtual void caret_height(int newval){ _height = newval; }

      virtual void create_caret() const {
        wtf::exception::throw_lasterr_if(::CreateCaret(*this, nullptr, _width, _height), [](BOOL b) { return !b; });
      }

      virtual void destroy_caret() const {
        wtf::exception::throw_lasterr_if(::DestroyCaret(), [](BOOL b) { return !b; });
      }

      virtual void show_caret() const {
        wtf::exception::throw_lasterr_if(::ShowCaret(*this), [](BOOL b) { return !b; });
      }

      virtual void hide_caret() const {
        wtf::exception::throw_lasterr_if(::HideCaret(*this), [](BOOL b) { return !b; });
      }

      virtual UINT caret_blink_rate() const { return _blink_rate; }

      virtual void caret_blink_rate(UINT newval) {
        _blink_rate = newval;
        wtf::exception::throw_lasterr_if(::SetCaretBlinkTime(newval), [](BOOL b) { return !b; });
      }

      virtual void caret_position(const point::client_coords &pos) {
        _pos = pos;
        wtf::exception::throw_lasterr_if(::SetCaretPos(_pos.x, _pos.y), [](BOOL b) { return !b; });
      }

      virtual point::client_coords caret_position() const{ return _pos; }

    protected:
      virtual LRESULT handle_message(HWND , UINT umsg, WPARAM , LPARAM , bool &) override {
        if (WM_SETFOCUS == umsg) {
          std::cout << "make caret" << std::endl;
          create_caret();
          caret_position(_pos);
          show_caret();
          caret_blink_rate(_blink_rate);
        } else if (WM_KILLFOCUS == umsg) {
          std::cout << "destroy caret" << std::endl;
          destroy_caret();
        }
        return 0;
      }
    private:
      UINT _blink_rate;
      point::client_coords _pos;
      int _width;
      int _height;
    };
  }
}
