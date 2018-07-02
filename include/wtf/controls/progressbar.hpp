/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

#define DOXY_INHERIT_PROGRESSBAR_SUPER \
  DOXY_INHERIT_WINDOW \
  DOXY_INHERIT_HAS_TEXT \
  DOXY_INHERIT_HAS_FONT \
  DOXY_INHERIT_HAS_ENABLE \
  DOXY_INHERIT_HAS_MOVE \
  DOXY_INHERIT_HAS_STYLE \
  DOXY_INHERIT_WM_COMMAND

namespace wtf {
  namespace controls {

    /** @class progressbar
    @brief Displays the progress of an operation
    @ingroup Controls
    */
    struct progressbar : DOXY_INHERIT_PROGRESSBAR_SUPER window_impl<progressbar,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_style,
      messages::wm_command
    > {

      void set_range(uint32_t min, uint32_t max) { ::SendMessage(*this, PBM_SETRANGE32, min, max); }
           
      uint32_t value() const { return static_cast<uint32_t>(::SendMessage(*this, PBM_GETPOS, 0, 0)); }

      void value(uint32_t newval) { ::SendMessage(*this, PBM_SETPOS, newval, 0); }

      //! @brief determines if smooth style is enabled
      bool smooth() const { return (window::_style & PBS_SMOOTH ? true : false); }
      //! @brief sets the smooth style
      //! @details NOTE: this style can only be set prior to calling run()
      void smooth(bool newval) {
        window::_style &= ~PBS_SMOOTH;
        window::_style |= (newval ? PBS_SMOOTH : 0);
      }

      //! @brief gets the orientation
      orientations orientation() const { return (window::_style & PBS_VERTICAL ? orientations::vertical : orientations::horizontal); }
      //! @brief sets the orientation
      //! @details NOTE: this style can only be set prior to calling run()
      void orientation(orientations newval) {
        window::_style &= ~PBS_VERTICAL;
        window::_style |= (orientations::vertical == newval ? PBS_VERTICAL : 0);
      }

      //! @brief gets the marquee style
      bool marquee() const { return (window::_style & PBS_MARQUEE ? true : false); }
      //! @brief sets the marquee style
      //! @details NOTE: this style can only be set prior to calling run()
      void marquee(bool newval) {
        window::_style &= ~PBS_MARQUEE;
        window::_style |= (newval ? PBS_MARQUEE : 0);
      }
    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      static constexpr TCHAR sub_window_class_name[] = PROGRESS_CLASS;

      static constexpr TCHAR window_class_name[] = _T("wtf_progressbar");

      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    
    };
  }
}
