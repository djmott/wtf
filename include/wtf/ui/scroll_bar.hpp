/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#if 0
namespace wtf{

  enum class scroll_visibilities{
    on,
    off,
    as_needed,
  };

  namespace policy {

    template <typename _super_t>
    struct isa_scroll_bar : _super_t {

      int min() const { return _min; }
      void min(int newval) {
        _min = newval;
        value(value());
      }

      int max() const { return _max; }
      void max(int newval) {
        _max = newval;
        value(value());
      }

      int value() const noexcept { return _value; }

      void value(int newval) {
        if (newval < _min) newval = _min;
        if (newval > _max) newval = _max;
        _value = newval;
        _super_t::invalidate();
      }

      int small_step() const { return _small_step; }
      void small_step(int newval) {
        _small_step = newval;
      }

      int big_step() const { return _big_step; }
      void big_step(int newval) {
        _big_step = newval;
      }

      callback<void(int prev_val)> OnValueChanged;

    protected:

      explicit isa_scroll_bar(window * pParent) : _super_t(pParent) {}

      virtual void on_value_changed(int prev_val) { OnValueChanged(prev_val); }

    private:

      struct page : window_impl<page,
        policy::isa_button> {};
    };

  }
  struct scroll_bar : window_impl<scroll_bar, 
    policy::isa_scroll_bar,
    policy::isa_panel, 
    policy::has_repeat_click,
    policy::has_orientation,
    policy::has_click,
    policy::has_timer,
    policy::has_move,
    policy::has_invalidate,
    policy::wm_erasebkgnd,
    policy::wm_mouse_wheel,
    policy::wm_mouse_move,
    policy::wm_mouse_down,
    policy::wm_mouse_up,
    policy::wm_mouse_leave,
    policy::wm_paint,
    policy::wm_timer
  >{
    explicit scroll_bar(window * pParent)  : window_impl(pParent){}
  };

}
#endif