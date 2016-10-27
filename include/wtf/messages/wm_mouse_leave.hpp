#pragma once

namespace wtf{

    /** has_mouse_leave
    * produces mouse leave events
    */
    template <typename _ImplT, policy..._Policies>
    class window<_ImplT, policy::wm_mouse_leave, _Policies...> 
      : public window_impl<_ImplT, _Policies...>
    {
      using __super_t = window_impl<_ImplT, _Policies...>;
      template <typename, policy ... > friend class window_impl;
    public:


    protected:

      virtual void on_wm_mouse_leave() {}

      explicit window(iwindow * pParent) : __super_t(pParent){}

      virtual void handle_msg(window_message& msg) override{
        if (WM_CREATE == msg.umsg){
          TRACKMOUSEEVENT oEvent;
          memset(&oEvent, 0, sizeof(TRACKMOUSEEVENT));
          oEvent.cbSize = sizeof(TRACKMOUSEEVENT);
          oEvent.dwFlags = TME_LEAVE;
          oEvent.hwndTrack = hwnd;
          wtf::exception::throw_lasterr_if(::TrackMouseEvent(&oEvent), [](BOOL b){return !b; });
        } else if (WM_MOUSELEAVE == msg.umsg){
          on_wm_mouse_leave();
        }
      }

    private:
    };
  }
