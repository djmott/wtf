/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct has_background : _SuperT{

      virtual const brush& background_brush() const{ return _background_brush; }
      virtual void background_brush(brush&& newval){ _background_brush = std::move(newval); }

    protected:

      explicit has_background(iwindow * pParent) : _SuperT(pParent){}

      void on_wm_erasebkgnd(const _::device_context& dc, const rect<coord_frame::client>& client) override{
          dc.fill(client, background_brush());
        _SuperT::on_wm_erasebkgnd(dc, client);
      }

    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
  }

  namespace _{
    template <> struct policy_traits<policy::has_background>{
      using requires = policy_list<policy::wm_erasebkgnd>;
    };
  }
}