#pragma once
namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_repeat_click : _SuperT{

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:

      virtual void TimerEvent(UINT_PTR iTimer){
        if (iTimer == _timerid){
          ClickEvent(point::client_coords(0,0));
          set_timer(_repeat_rate, iTimer);
        }
      }

      virtual void MouseMoveEvent(event_vkeys k, const point::client_coords& p)override{
        auto client = rect::client_coord::get(*this);
        if (!client.is_in(p) && _down && _timerid){
          kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      }

      virtual void MouseLButtonDownEvent(event_vkeys k, const point::client_coords& p) override{
        _down = true;
        _timerid = set_timer(_repeat_delay);
      }

      virtual void MouseLButtonUpEvent(event_vkeys k, const point::client_coords& p) override{
        if (_down && _timerid){
          kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      }

    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 250;
      int _repeat_rate = 50;
    };
  }
}