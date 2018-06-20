/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace custom {

    struct splitter : wtf::window_impl<splitter,
      wtf::policy::has_move,
      wtf::policy::has_orientation,
      wtf::policy::has_size,
      wtf::policy::wm_size,
      wtf::policy::wm_showwindow
    > {

      static constexpr DWORD Style = WS_VISIBLE | WS_CHILD;

      template <typename _first_t, typename _second_t>
      splitter(_first_t& first, _second_t& second) : slider(this), _first(&first), _second(&second) {
        add(slider);
        add(first);
        add(second);
      }

      int slider_width() const { return _slider_width; }
      void slider_width(int newval) { _slider_width = newval; }

      void set_split_absolute(int pixels) {
        _slider_pos = pixels;
        _reposition();
      }

      void set_split_relative(int percent) {
        if (orientations::horizontal == __super::orientation()) {
          _slider_pos = (percent * height() / 100);
        }
        else {
          _slider_pos = (percent * width() / 100);
        }
        _reposition();
      }

    protected:
      window * _first;
      window * _second;
      int _slider_width = 10;
      int _slider_pos = 0;



      struct slider_t : wtf::window_impl<slider_t,
        wtf::policy::has_cursor,
        wtf::policy::has_move,
        wtf::policy::wm_setcursor,
        wtf::policy::wm_mouse_move,
        wtf::policy::wm_mouse_down,
        wtf::policy::wm_mouse_up
      > {
        friend struct splitter;

        static constexpr DWORD Style = WS_CHILD | WS_VISIBLE;

        slider_t(splitter * Parent) : _splitter(Parent) {}
        splitter * _splitter;

        const wtf::cursor &cursor_pointer() const override {
          if (wtf::orientations::horizontal == _splitter->orientation()) {
            return cursor::global(cursor::style::size_ns);
          }
          else {
            return cursor::global(cursor::style::size_we);
          }
        }

        void on_wm_mouse_move(const mouse_msg_param<coord_frame::client>& m) override {
          if (!(m.key_state & mouse_key_states::left)) return;
          _splitter->_slider_pos += (wtf::orientations::horizontal == _splitter->orientation() ? m.position.y : m.position.x);
          _splitter->_reposition();
        }

        void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& m) override {
          if (mouse_buttons::left == m.button) SetCapture(*this);
          _splitter->_reposition();
        };

        void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& m) override {
          ReleaseCapture();
          _splitter->_reposition();
        };


      }slider;

      void _reposition() {
        if (_slider_pos < 10) _slider_pos = 10;

        if (wtf::orientations::horizontal == __super::orientation()) {
          if (_slider_pos > height() - 10) _slider_pos = height() - 10;
          ::MoveWindow(*_first, 0, 0, width(), _slider_pos, TRUE);
          ::MoveWindow(*_second, 0, _slider_pos + _slider_width, width(), height() - _slider_pos - _slider_width, TRUE);
          ::MoveWindow(*slider, 0, _slider_pos, width(), _slider_width, TRUE);
        }else {
          if (_slider_pos > width() - 10) _slider_pos = width() - 10;
          ::MoveWindow(*_first, 0, 0, _slider_pos, height(), TRUE);
          ::MoveWindow(*_second, _slider_pos + _slider_width, 0, width() - _slider_pos - _slider_width, height(), TRUE);
          ::MoveWindow(*slider,_slider_pos, 0, _slider_width, height(), TRUE);
        }


      };

      void on_wm_size(const point<coord_frame::client>& p) override {
        _reposition();
        __super::on_wm_size(p);
      }



    };
  }
}

