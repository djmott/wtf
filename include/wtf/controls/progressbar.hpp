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
  namespace policy {

    /** @class progressbar_impl
    @brief Default progress bar implementation
    @tparam _impl_t the concrete implementation
    @tparam _orientation Indicates a vertical or horizontal progress bar
    @ingroup Policies
    */
    template <typename _impl_t, orientations _orientation>
    struct progressbar_impl : DOXY_INHERIT_PROGRESSBAR_SUPER window_impl<_impl_t,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_style,
      messages::wm_command
    > {

      void set_range(uint32_t min, uint32_t max) { ::SendMessage(*this, PBM_SETRANGE32, min, max); }
           
      uint32_t value() const { return ::SendMessage(*this, PBM_GETPOS, 0, 0); }

      void value(uint32_t newval) { ::SendMessage(*this, PBM_SETPOS, newval, 0); }

    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      static constexpr DWORD Style = window::Style | PBS_SMOOTH | (orientations::vertical == _orientation ? PBS_VERTICAL : 0);
      static constexpr TCHAR sub_window_class_name[] = PROGRESS_CLASS;

    };
  }

  namespace controls{
    /** @class hprogressbar
    @brief a horizontal progress bar
    @ingroup Controls
    */
    struct hprogressbar : policy::progressbar_impl<hprogressbar, orientations::horizontal> {
    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      static constexpr TCHAR window_class_name[] = _T("wtf_hprogressbar");

      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    };
    /** @class vprogressbar
    @brief a vertical progress bar
    @ingroup Controls
    */
    struct vprogressbar : policy::progressbar_impl<vprogressbar, orientations::vertical> {
    protected:
      template <typename, template <typename> typename...> friend struct window_impl;
      static constexpr TCHAR window_class_name[] = _T("wtf_vprogressbar");


      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    };
  }

}
