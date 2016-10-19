#pragma once
namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_repeat_click : _SuperT {

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      has_repeat_click(window<void> * pParent) : _SuperT(pParent){
      }
      virtual void wm_timer(UINT_PTR iTimer){
        if (iTimer == _timerid){
          _SuperT::wm_click(policy::mouse_event((WPARAM)0, (LPARAM)0, policy::mouse_event::buttons::left));
          set_timer(_repeat_rate, iTimer);
        }
      };
      virtual void wm_mouse_move(const policy::mouse_event& m) {
        auto client = rect<coord_frame::client>::get(*this);
        if (!client.is_in(m.position) && _down && _timerid){
          _SuperT::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      };
      virtual void wm_mouse_down(const policy::mouse_event& m) {
        if (policy::mouse_event::buttons::left != m.button) return;
        _down = true;
        _timerid = _SuperT::set_timer(_repeat_delay);
      };
      virtual void wm_mouse_up(const policy::mouse_event& m) {
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