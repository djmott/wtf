#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class has_background : public _SuperT{

    public:

      virtual const brush& background_brush() const{ return _background_brush; }
      virtual void background_brush(brush&& newval){ _background_brush = std::move(newval); }

    protected:

      void on_wm_erasebkgnd(const device_context& dc, const rect<coord_frame::client>& client) override{
          dc.fill(client, background_brush());
        _SuperT::on_wm_erasebkgnd(dc, client);
      }

      explicit has_background(iwindow * pParent) : _SuperT(pParent){}

    private:
      brush _background_brush = brush::solid_brush(system_colors::button_face);
    };
  }

  template <> struct policy_traits<policy::has_background>{
    using requires = policy_list<policy::wm_erasebkgnd>;
  };

}