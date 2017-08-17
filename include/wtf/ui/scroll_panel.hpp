#pragma once

namespace wtf{
  namespace policy{


    template <typename _SuperT>
    struct isa_scroll_panel : _SuperT{

      scroll_bar& hscroll() { return _hscroll; }
      const scroll_bar& hscroll() const { return _hscroll; }

      scroll_bar& vscroll() { return _vscroll; }
      const scroll_bar& vscroll() const { return _vscroll; }

      panel& client() { return _client; }
      const panel& client() const { return _client; }

    protected:

      static const int scrollbar_size = 20;

      explicit isa_scroll_panel(window * pParent) : _SuperT(pParent), _client(this), _hscroll(this), _vscroll(this){
        _hscroll.orientation(orientations::horizontal);
        _vscroll.orientation(orientations::vertical);
      }



      void on_wm_size(const point<coord_frame::client>& client) override {
        auto border = _SuperT::border_width();
        bool hscroll_visible = _hscroll.visible();
        bool vscroll_visible = _vscroll.visible();

        if (hscroll_visible) {
          _hscroll.move(0, client.y - scrollbar_size, (vscroll_visible ? client.x - scrollbar_size : client.x), scrollbar_size);
        }
        if (vscroll_visible) {
          _vscroll.move(client.x - scrollbar_size, 0, scrollbar_size, (hscroll_visible ? client.y - scrollbar_size : client.y));
        }
        _client.move(0, 0, (vscroll_visible ? client.x - scrollbar_size : client.x), (hscroll_visible ? client.y - scrollbar_size : client.y));
        _SuperT::on_wm_size(client);
      }


      scroll_bar _hscroll;
      scroll_bar _vscroll;
      panel _client;

    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_scroll_panel>{
      using requires = policy_list<policy::isa_panel>;
    };
  }


  class vscroll_panel : public window_impl<vscroll_panel, policy::isa_scroll_panel> {
    using __super_t = window_impl<vscroll_panel, policy::isa_scroll_panel>;
  public:
    vscroll_panel(window * parent) : __super_t(parent) {}

  };

}
