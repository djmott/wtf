/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace policy{

    /** has_caret
    * Controls the caret of text/input elements
    */
    template <typename _super_t>
    struct has_caret : _super_t{
      
      has_caret() = default;

      virtual int caret_width() const noexcept { return _width; }
      virtual void caret_width(int newval) noexcept { _width = newval; }

      virtual int caret_height() const noexcept { return _height; }
      virtual void caret_height(int newval) noexcept { _height = newval; }

      virtual UINT caret_blink_rate() const noexcept { return _blink_rate; }

      virtual void caret_blink_rate(UINT newval)  {
        _blink_rate = newval;
        _has_focus && wtf::exception::throw_lasterr_if(::SetCaretBlinkTime(newval), [](BOOL b)noexcept { return !b; });
      }

    protected:

      virtual void create_caret()  {
        _has_focus && wtf::exception::throw_lasterr_if(::CreateCaret(*this, nullptr, _width, _height), [](BOOL b)noexcept { return !b; });
        _caret_visible = false;
      }

      virtual void destroy_caret()  {
        wtf::exception::throw_lasterr_if(::DestroyCaret(), [](BOOL b)noexcept { return !b; });
        _caret_visible = false;
      }

      virtual bool caret_visible() const noexcept { return _caret_visible; }
      virtual void caret_visible(bool newval)  {
        if (_caret_visible == newval) return;
        _caret_visible = newval;
        if (_caret_visible){
          _has_focus && wtf::exception::throw_lasterr_if(::HideCaret(*this), [](BOOL b) noexcept { return !b; });
        } else{
          _has_focus && wtf::exception::throw_lasterr_if(::ShowCaret(*this), [](BOOL b) noexcept { return !b; });
        }
      }

      virtual void caret_position(const point<coord_frame::client> &pos)  {
        _pos = pos;
        _has_focus && wtf::exception::throw_lasterr_if(::SetCaretPos(_pos.x, _pos.y), [](BOOL b) noexcept { return !b; });
      }

      virtual point<coord_frame::client> caret_position() const noexcept { return _pos; }


      void on_wm_setfocus(HWND hwnd) override{
        _has_focus = true;
        create_caret();
        caret_position(_pos);
        caret_visible(true);
        caret_blink_rate(_blink_rate);
        _super_t::on_wm_setfocus(hwnd);
      }

      void on_wm_killfocus(HWND hwnd) override{
        _has_focus = false;
        destroy_caret();
        _super_t::on_wm_killfocus(hwnd);
      }

    private:

      UINT _blink_rate = 250;
      point<coord_frame::client> _pos = point<coord_frame::client>(0, 0);
      bool _caret_visible = false;
      int _width = 1;
      int _height = 1;
      bool _has_focus = false;
    };
  }

}