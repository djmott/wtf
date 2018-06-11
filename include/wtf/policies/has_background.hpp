/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _super_t>
    struct has_background : _super_t{

      virtual const brush& background_brush() const noexcept { return _background_brush; }
      virtual void background_brush(brush&& newval) noexcept { _background_brush = std::move(newval); }

    protected:

      explicit has_background(window * pParent)  : _super_t(pParent){}

      void on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& client) {
          dc.fill(client, background_brush());
        _super_t::on_wm_erasebkgnd(dc, client);
      }

    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
  }

}