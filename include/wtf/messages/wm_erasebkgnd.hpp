#pragma once

namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::wm_erasebkgnd, _Policies...>
    : public window_impl<_ImplT, _Policies...>{
    using __super_t = window_impl<_ImplT, _Policies...>;
    template <typename, policy ... > friend class window_impl;
  public:

    virtual const brush& background_brush() const{ return _background_brush; }
    virtual void background_brush(brush&& newval){ _background_brush = std::move(newval); }

  protected:

    virtual void handle_msg(window_message& msg) override{
      if (WM_ERASEBKGND == msg.umsg){
        auto &dc = *reinterpret_cast<const device_context *>(msg.lparam);
        dc.fill(rect<coord_frame::client>::get(*this), background_brush());
        msg.lresult = TRUE;
        msg.bhandled = true;
      }
    }

    explicit window(iwindow * pParent) : __super_t(pParent){}

  private:
    brush _background_brush = brush::solid_brush(system_colors::button_face);
  };
}
