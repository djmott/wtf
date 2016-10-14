#pragma once
namespace wtf{
  struct timer : window<timer, policy::has_timer>{
    timer(HWND parent) : window(parent){}

    callback<void()> OnTimer;

  protected:
    virtual void TimerEvent(UINT_PTR) override{ OnTimer(); }

  };
}