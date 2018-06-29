/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#if 0
#if !DOXY_INVOKED 
#define DOXY_INHERIT_AVI_SUPER \
DOXY_INHERIT_HAS_ENABLE \
DOXY_INHERIT_HAS_BORDER \
DOXY_INHERIT_HAS_FONT \
DOXY_INHERIT_HAS_TEXT \
DOXY_INHERIT_WM_COMMAND \
DOXY_INHERIT_HAS_MOVE

namespace wtf {
  namespace controls {


    /** @class avi_player
    @brief displays an Audio-Video Interleaved (AVI) clip
    @ingroup Widgets
    */
    struct avi_player : DOXY_INHERIT_AVI_SUPER window_impl<avi_player,
      policy::has_enable,
      policy::has_border,
      policy::has_font,
      policy::has_text,
      policy::wm_command,
      policy::has_move
    >{

      //! @brief invoked when the clip starts
      callback<void(window*)> OnStart;
      //! @brief invoked when the clip stops
      callback<void(window*)> OnStop;

      //! @brief opens an .avi file
      //! @param[in] path full path to the avi clip
      void open(const tstring& path) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_OPEN, 0, reinterpret_cast<LPARAM>(path.c_str())), [](LRESULT l) { return !l; });
      }

      //! @brief starts playing the .avi clip
      //! @param[in] first,last frames of the clip to play
      void start(uint16_t first = 0, uint16_t last = 0xffff) {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_PLAY, 0, MAKELONG(first, last)), [](LRESULT l) { return !l; });
      }

      //! @brief stops playing the .avi clip
      void stop() {
        wtf::exception::throw_lasterr_if(::SendMessage(*this, ACM_STOP, 0, 0), [](LRESULT l) { return !l; });
      }

      //! @brief indicates whether the .avi clip is playing
      //! @returns true if the clip is playing or false if it is not playing
      bool is_playing() const {
        return ::SendMessage(*this, ACM_ISPLAYING, 0, 0) ? true : false;
      }

    protected:
#if !defined(DOXY_INVOKED)
      void on_wm_command(WPARAM wparam, LPARAM lparam) override{
        if (lparam != reinterpret_cast<LPARAM>(this->_handle)) return;
        if (ACN_START == HIWORD(wparam)) OnStart(this);
        if (ACN_STOP == HIWORD(wparam)) OnStart(this);
      }
#endif
    };

  }

#if !DOXY_INVOKED

  namespace _ {

    TCHAR sANIMATE_CLASS[] = ANIMATE_CLASS;

  }

  template <WNDPROC window_proc>
  struct window_class<controls::avi_player, window_proc> :
    super_window_class<_::sANIMATE_CLASS, controls::avi_player, window_proc> {};
#endif

}
#endif
#endif