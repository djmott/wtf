#pragma once

namespace wtf{
  namespace policy{

    template<typename _SuperT, typename _ImplT>
    struct has_background : _SuperT{

      const brush& background_brush() const{ return _background_brush; }
      void background_brush(brush&& newval){ _background_brush = std::move(newval); }

    protected:

      virtual LRESULT on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& client, bool& bHandled) override{
        dc.fill(client, background_brush());
        return TRUE;
      }


      explicit has_background(window<void,void> * pParent) : _SuperT(pParent){}
    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
  }
}
