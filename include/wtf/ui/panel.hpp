#pragma once

namespace wtf{

  namespace policy{
    template <typename _SuperT>
    class isa_panel : public _SuperT{

    protected:

      explicit isa_panel(iwindow * pParent) : _SuperT(pParent){}
      
      void on_wm_create() override{ _SuperT::on_wm_create(); }
      void on_wm_paint(const device_context& dc, const paint_struct& ps) override{ _SuperT::on_wm_paint(dc, ps); }
      void on_wm_size(const point<coord_frame::client>& pt) override{ _SuperT::on_wm_size(pt); }

    };
  }

  template <> struct policy_traits<policy::isa_panel>{
    using requires = policy_list<policy::has_border, policy::has_click,  policy::has_show,
    policy::has_size, policy::has_move,  policy::has_invalidate, policy::has_background,
    policy::has_zorder, policy::wm_size, policy::wm_paint, policy::wm_create>;
  };


  struct panel : window_impl<panel, policy::isa_panel>{
    explicit panel(iwindow * pParent) : window_impl(pParent){}
  };

}
