/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{

  namespace policy{

    template <typename _super_t>
    struct has_repeat_click : _super_t{
      
      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      explicit has_repeat_click(window * pParent)  : _super_t(pParent){}

      void on_wm_click(const mouse_msg_param<coord_frame::client>& p) override {
        _super_t::on_wm_click(p);
      }
      
      void on_wm_timer(UINT_PTR iTimer) override{
        if (iTimer == _timerid){
          on_wm_click(mouse_msg_param<coord_frame::client>((WPARAM)0, (LPARAM)0, mouse_buttons::left));
          _super_t::set_timer(_repeat_rate, iTimer);
        }
        _super_t::on_wm_timer(iTimer);
      }

      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& m) override{
        if (mouse_buttons::left != m.button) return;
        _down = true;
        _timerid = _super_t::set_timer(_repeat_delay);
        _super_t::on_wm_mouse_down(m);
      }

      void on_wm_mouse_up(const mouse_msg_param<coord_frame::client>& m) override{
        if (_down && _timerid){
          if (_timerid) _super_t::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        _super_t::on_wm_mouse_up(m);
      };

      void on_wm_mouse_leave() override{ 
        if (_timerid) _super_t::kill_timer(_timerid);
        _timerid = 0;
        _down = false;
        _super_t::on_wm_mouse_leave();
      }


    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }

}