#pragma once
namespace wtf{
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::has_repeat_click, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:

      using mouse_msg_param = mouse_msg_param;

      int repeat_delay() const{ return _repeat_delay; }
      void repeat_delay(int newval){ _repeat_delay = newval; }

      int repeat_rate() const{ return _repeat_rate; }
      void repeat_rate(int newval){ _repeat_rate = newval; }

    protected:
      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void wm_timer(UINT_PTR iTimer){
        if (iTimer == _timerid){
          __super_t::on_wm_click(mouse_msg_param((WPARAM)0, (LPARAM)0, mouse_msg_param::buttons::left));
          set_timer(_repeat_rate, iTimer);
        }
        __super_t::wm_timer(iTimer);
      }

      virtual void wm_mouse_move(const mouse_msg_param& m) {
        auto client = rect<coord_frame::client>::get(*this);
        if (!client.is_in(m.position) && _down && _timerid){
          __super_t::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        __super_t::wm_mouse_move(m);
      }

      virtual void wm_mouse_down(const mouse_msg_param& m) {
        if (mouse_msg_param::buttons::left != m.button) return;
        _down = true;
        _timerid = __super_t::set_timer(_repeat_delay);
        __super_t::wm_mouse_down(m);
      }

      virtual void wm_mouse_up(const mouse_msg_param& m) {
        if (_down && _timerid){
          __super_t::kill_timer(_timerid);
          _timerid = 0;
          _down = false;
        }
        __super_t::wm_mouse_up(m);
      };

    private:
      UINT_PTR _timerid = 0;
      bool _down = false;
      int _repeat_delay = 500;
      int _repeat_rate = 50;
    };
  }
