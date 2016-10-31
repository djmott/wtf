#pragma once
namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class has_repeat_click : public _SuperT{
      
    public:

      using mouse_msg_param = mouse_msg_param;

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      explicit has_repeat_click(iwindow * pParent) : _SuperT(pParent){}

      void wm_timer(UINT_PTR iTimer) override{
        if (iTimer == _timerid){
          _SuperT::on_wm_click(mouse_msg_param((WPARAM)0, (LPARAM)0, mouse_msg_param::buttons::left));
	        _SuperT::set_timer(_repeat_rate, iTimer);
        }
        _SuperT::wm_timer(iTimer);
      }

      void wm_mouse_move(const mouse_msg_param& m) override{
        auto client = rect<coord_frame::client>::get(*this);
        if (!client.is_in(m.position) && _down && _timerid){
          _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        _SuperT::wm_mouse_move(m);
      }

      void wm_mouse_down(const mouse_msg_param& m) override{
        if (mouse_msg_param::buttons::left != m.button) return;
        _down = true;
        _timerid = _SuperT::set_timer(_repeat_delay);
        _SuperT::wm_mouse_down(m);
      }

      void wm_mouse_up(const mouse_msg_param& m) override{
        if (_down && _timerid){
          _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        _SuperT::wm_mouse_up(m);
      };

    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }
}