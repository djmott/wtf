/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{
  namespace messages {

    template <typename _super_t>
    struct wm_paint : _super_t{

      callback<void(const device_context&, const paint_struct&)> OnPaint;

    protected:

      template <typename, template <typename> typename...> friend struct window_impl;
      
      virtual void on_wm_paint(const device_context& dc, const paint_struct& ps) { OnPaint(dc, ps); }


      void handle_msg(wtf::window_message& msg) override {
        if (WM_PAINT != msg.umsg) return;
        msg.lresult = 0;
        msg.bhandled = true;
        RECT r;
        if (0 == ::GetUpdateRect(*this, &r, FALSE)) return;
        on_wm_paint(device_context::get_client(*this), paint_struct(*this));
      }
    
    };
  }
}