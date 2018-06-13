/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace _ {

      TCHAR sANIMATE_CLASS[] = ANIMATE_CLASS;

      template <typename _impl_t> using avi_player_impl = window_impl<_impl_t,
        policy::has_font,
        policy::has_text,
        wtf::policy::wm_command,
        wtf::policy::has_move
      >;

    }

    struct avi_player : _::avi_player_impl<avi_player> {


      avi_player(window * parent) : _::avi_player_impl<avi_player>(parent) {
        wtf::_::init_common_controls<wtf::_::animate_classes>::get();
      }

      callback<void(window*)> OnStart;
      callback<void(window*)> OnStop;

      void open(const tstring& path) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_OPEN, 0, reinterpret_cast<LPARAM>(path.c_str())), [](LRESULT l) { return 0 == l; });
      }

      void start(uint16_t first = 0, uint16_t last = 0xffff) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_PLAY, 0, MAKELONG(first, last)), [](LRESULT l) { return 0 == l; });
      }

      void stop() {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_STOP, 0, 0), [](LRESULT l) { return 0 == l; });
      }

      bool is_playing() const {
        return ::SendMessage(*this, ACM_ISPLAYING, 0, 0) ? true : false;
      }

    protected:
      void on_wm_command(WPARAM wparam, LPARAM lparam) override{
        if (lparam != reinterpret_cast<LPARAM>(this->_handle)) return;
        if (ACN_START == HIWORD(wparam)) OnStart(this);
        if (ACN_STOP == HIWORD(wparam)) OnStart(this);
      }

    };

  }

  template <WNDPROC window_proc>
  struct window_class<controls::avi_player, window_proc> :
    super_window_class<controls::_::sANIMATE_CLASS, controls::avi_player, window_proc> {};

}