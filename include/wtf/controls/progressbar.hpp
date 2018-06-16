/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {
  namespace controls {

    namespace policy {

      template <typename _impl_t> using progressbar_super_t = window_impl<_impl_t,
        policy::has_text,
        policy::has_font,
        wtf::policy::has_enable,
        wtf::policy::has_move,
        wtf::policy::wm_command
      >;
    }

    /** @class progressbar
    @ingroup Widgets
    @brief A progress bar is a window that an application can use to indicate the progress of a lengthy operation.
    @tparam _orientation Indicates a vertical or horizontal progress bar
    @tparam _smooth Indicates the progress bar should display a smooth or blocked progress.
    */
    template <orientations _orientation, bool _smooth = true>
    struct progressbar : policy::progressbar_super_t<progressbar<_orientation, _smooth>> {

      static constexpr DWORD Style = window::Style |
        (orientations::vertical == _orientation ? PBS_VERTICAL : 0) |
        (_smooth ? PBS_SMOOTH : 0);

      progressbar() : policy::progressbar_super_t<progressbar>() {}

      void set_range(uint32_t min, uint32_t max) { ::SendMessage(*this, PBM_SETRANGE32, min, max); }


      static constexpr orientations orientation() noexcept { return _orientation; }


      uint32_t value() const { return ::SendMessage(*this, PBM_GETPOS, 0, 0); }

      void value(uint32_t newval) { ::SendMessage(*this, PBM_SETPOS, newval, 0); }

    };


    template <orientations _orientation, bool _smooth = true>
    struct marquee : progressbar<_orientation, _smooth> {
 
      static constexpr DWORD Style = progressbar<_orientation, _smooth>::Style | PBS_MARQUEE;

      explicit marquee(window * pParent) : progressbar<_orientation, _smooth>(pParent) {}

      void set_marque(bool on, uint32_t speedms = 30) { ::SendMessage(*this, PBM_SETMARQUEE, (on ? -1 : FALSE), speedms); }

    };
  }

  namespace _ { TCHAR sPROGRESS_CLASS[] = PROGRESS_CLASS; }

  template <orientations _orientation, bool _smooth, WNDPROC window_proc>
  struct window_class<controls::progressbar<_orientation, _smooth>, window_proc> :
    super_window_class<_::sPROGRESS_CLASS, controls::progressbar<_orientation, _smooth>, window_proc> {};

  template <orientations _orientation, bool _smooth, WNDPROC window_proc>
  struct window_class<controls::marquee<_orientation, _smooth>, window_proc> :
    super_window_class<_::sPROGRESS_CLASS, controls::marquee<_orientation, _smooth>, window_proc> {};


}
