/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct wm_erasebkgnd : _SuperT{
      
    protected:

      virtual void on_wm_erasebkgnd(const wtf::_::device_context&, const rect<coord_frame::client>&) {}
      explicit wm_erasebkgnd(window * pParent) : _SuperT(pParent){}

      void handle_msg(_::window_message& msg) override{
        if (WM_ERASEBKGND == msg.umsg){
          on_wm_erasebkgnd(*reinterpret_cast<const wtf::_::device_context *>(msg.lparam), rect<coord_frame::client>::get(*this));
          msg.lresult = TRUE;
          msg.bhandled = true;
        }
        _SuperT::handle_msg(msg);
      }

    };
  }
}