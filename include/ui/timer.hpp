#pragma once
namespace wtf{
  struct timer : window<timer, policy::has_timer>{
    explicit timer(iwindow * pParent) : window(pParent){}

    callback<void()> OnTimer;

  protected:
    virtual void TimerEvent(UINT_PTR) override{ OnTimer(); }

  };
}