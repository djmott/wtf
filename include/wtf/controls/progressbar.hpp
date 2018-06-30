/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace policy {

    /** @class progressbar_impl
    @ingroup Policies
    @brief A progress bar is a window that an application can use to indicate the progress of a lengthy operation.
    @tparam _orientation Indicates a vertical or horizontal progress bar
    @tparam _smooth Indicates the progress bar should display a smooth or blocked progress.
    */
    template <typename _impl_t, orientations _orientation>
    struct progressbar_impl : window_impl<_impl_t,
      policy::has_text,
      policy::has_font,
      policy::has_enable,
      policy::has_move,
      policy::has_style,
      policy::wm_command
    > {

      static constexpr DWORD Style = window::Style | (orientations::vertical == _orientation ? PBS_VERTICAL : 0);
      static constexpr TCHAR sub_window_class_name[] = PROGRESS_CLASS;

      bool smooth() const { return get_style_bit<PBS_SMOOTH>(); }
      void smooth(bool newval) { set_style_bit<PBS_SMOOTH>(newval); }


      void set_range(uint32_t min, uint32_t max) { ::SendMessage(*this, PBM_SETRANGE32, min, max); }


      static constexpr orientations orientation() noexcept { return _orientation; }


      uint32_t value() const { return ::SendMessage(*this, PBM_GETPOS, 0, 0); }

      void value(uint32_t newval) { ::SendMessage(*this, PBM_SETPOS, newval, 0); }

    };
  }

  namespace controls{
    struct hprogressbar : policy::progressbar_impl<hprogressbar, orientations::horizontal> {
      static constexpr TCHAR window_class_name[] = _T("wtf_hprogressbar");

      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    };
    struct vprogressbar : policy::progressbar_impl<vprogressbar, orientations::vertical> {
      static constexpr TCHAR window_class_name[] = _T("wtf_vprogressbar");


      template <WNDPROC wp> using window_class_type = super_window_class<window_class_name, sub_window_class_name, wp>;
    };
  }

}
