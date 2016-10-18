#pragma once

namespace wtf{
  namespace policy{
    template <typename _SuperT, typename>
    struct has_activate : _SuperT{
    protected:
      has_activate(iwindow * pParent) : _SuperT(pParent){}

      virtual void ActivatedEvent() = 0;
      virtual void DeactivatedEvent() = 0;

      LRESULT handle_message(HWND, UINT umsg, WPARAM wparam, LPARAM, bool &) {
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