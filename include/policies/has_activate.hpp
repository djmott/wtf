#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename>
    struct has_activate : _SuperT{
    protected:
      virtual void ActivatedEvent(){}
      virtual void DeactivatedEvent(){}

      virtual LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool &) override{
        if (WM_ACTIVATE == umsg){
          if (WA_INACTIVE == wparam){
            DeactivatedEvent();
          } else{
            ActivatedEvent();
          }
        }
        return 0;
      }

    };
  }
}