#pragma once
namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_repeat_click : _SuperT {
      using mouse_msg_param = messages::mouse_msg_param;

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      has_repeat_click(window<void> * pParent) : _SuperT(pParent){
      }
      virtual void wm_timer(UINT_PTR iTimer){
        if (iTimer == _timerid){
          _SuperT::wm_click(mouse_msg_param((WPARAM)0, (LPARAM)0, mouse_msg_param::buttons::left));
          set_timer(_repeat_rate, iTimer);
        }
      };
      virtual void wm_mouse_move(const mouse_msg_param& m) {
        auto client = rect<coord_frame::client>::get(*this);
        if (!client.is_in(m.position) && _down && _timerid){
          _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      };
      virtual void wm_mouse_down(const mouse_msg_param& m) {
        if (mouse_msg_param::buttons::left != m.button) return;
        _down = true;
        _timerid = _SuperT::set_timer(_repeat_delay);
      };
      virtual void wm_mouse_up(const mouse_msg_param& m) {
        if (_down && _timerid){
          _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      };

    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }
}