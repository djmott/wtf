#pragma once
namespace wtf{
  namespace policy{
    template <typename _SuperT, typename _ImplT> struct has_repeat_click : _SuperT{

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      has_repeat_click(iwindow * pParent) : _SuperT(pParent){}

      virtual void TimerEvent(UINT_PTR iTimer){
        _SuperT::TimerEvent(iTimer);
        if (iTimer == _timerid){
          ClickEvent(policy::mouse_event(mouse_event::buttons::left, mouse_event::key_states::unspecified, 0,0));
          set_timer(_repeat_rate, iTimer);
        }
      }

      virtual void MouseMoveEvent(const policy::mouse_event& m) override {
        _SuperT::MouseMoveEvent(m);
        auto client = rect::client_coord::get(*this);
        if (!client.is_in(m.position) && _down && _timerid){
          kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      }

      virtual void MouseDownEvent(const policy::mouse_event& m) override {
        _SuperT::MouseDownEvent(m);
        if (policy::mouse_event::buttons::left != m.button) return;
        _down = true;
        _timerid = set_timer(_repeat_delay);
      }

      virtual void MouseUpEvent(const policy::mouse_event& m) override {
        _SuperT::MouseUpEvent(m);
        if (_down && _timerid){
          kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
      }

    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }
}