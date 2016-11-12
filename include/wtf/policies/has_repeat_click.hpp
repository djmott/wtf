/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct has_repeat_click : _SuperT{
      
      using mouse_msg_param = mouse_msg_param;

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      explicit has_repeat_click(window * pParent) : _SuperT(pParent){}

      void on_wm_click(const mouse_msg_param& p) override {
        _SuperT::on_wm_click(p);
      }
      
      void on_wm_timer(UINT_PTR iTimer) override{
        if (iTimer == _timerid){
          on_wm_click(mouse_msg_param((WPARAM)0, (LPARAM)0, mouse_msg_param::buttons::left));
          _SuperT::set_timer(_repeat_rate, iTimer);
        }
        _SuperT::on_wm_timer(iTimer);
      }

      void on_wm_mouse_down(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return;
        _down = true;
        _timerid = _SuperT::set_timer(_repeat_delay);
        _SuperT::on_wm_mouse_down(m);
      }

      void on_wm_mouse_up(const mouse_msg_param& m) override{
        if (_down && _timerid){
          if (_timerid) _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        _SuperT::on_wm_mouse_up(m);
      };

      void on_wm_mouse_leave() override{ 
        if (_timerid) _SuperT::kill_timer(_timerid);
        _timerid = 0;
        _down = false;
        _SuperT::on_wm_mouse_leave();
      }


    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }


  namespace _{
    template <> struct policy_traits<policy::has_repeat_click>{
      using requires = policy_list<policy::has_click, policy::has_timer, policy::wm_mouse_leave>;
    };
  }

}