#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT, typename _ImplT>
    class wm_erasebkgnd : public _SuperT{
      
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
        _SuperT::handle_msg(msg);
      }

      explicit wm_erasebkgnd(iwindow * pParent) : _SuperT(pParent){}

    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
  }
}