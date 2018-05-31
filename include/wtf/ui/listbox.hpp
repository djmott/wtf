/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf {

  enum class item_selection_modes {
    none,
    single,
    multiple,
    extended,
  };

  namespace policy {
    
    template <typename _SuperT>
    struct isa_listbox : _SuperT {

      item_selection_modes selection_mode() const { return _selection_mode; }
      void selection_mode(item_selection_modes newval) { _selection_mode = newval; }
      
      struct item : std::enable_shared_from_this<item>{
        using pointer = std::shared_ptr<item>;

        tstring& text() { return _text; }
        const tstring& text() const { return _text; }
        void text(const tstring& newval) { 
          _text = newval; 
          if (_parent) _parent->invalidate();
        }

        bool selected() const { return _selected; }
        void selected(bool newval) { 
          _selected = newval; 
          if (_parent) _parent->invalidate();
        }

      private:
        tstring _text = _T("");
        bool _selected = false;
        isa_listbox<_SuperT> * _parent = nullptr;
      };

      void add_item(const tstring& newval) {
        _items.push_back(newval);
        resize();
      }

      void add_items(const std::initializer_list<tstring> items) {
        std::copy(items.begin(), items.end(), std::back_insert_iterator<std::vector<tstring>>(_items));
        resize();
      }

    protected:

      isa_listbox(window * parent) : _SuperT(parent), _contents(*this){}


      void on_wm_size(const point<coord_frame::client>& p) override {
        _current_size = p;
        resize();
        _SuperT::on_wm_size(p);
      }



    private:
      friend struct contents;

      item_selection_modes _selection_mode;
      std::vector<tstring> _items;

      point<coord_frame::client> _current_size;

      void resize() {
        auto oExtent = _contents.calc_size();
        bool vscroll_visible = oExtent.cy > _current_size.y;
        bool hscroll_visible = oExtent.cx > _current_size.x;
        _SuperT::vscroll().max(_current_size.y);
        _SuperT::hscroll().max(_current_size.x);
        _SuperT::vscroll().visible(vscroll_visible);
        _SuperT::hscroll().visible(hscroll_visible);
        _contents.move(_SuperT::hscroll().value(), _SuperT::vscroll().value(), oExtent.cx, oExtent.cy);
      }



      struct contents : label {

        contents(isa_listbox& parent) : label(&parent.client()), _parent(parent) {
          label::multiline(true);
          label::word_wrap(false);
          label::text_vertical_alignment(text_vertical_alignments::top);
          label::text_horizontal_alignment(text_horizontal_alignments::left);
          label::auto_draw_text(false);
        }


        size calc_size() const {
          tstring sList = _T("");
          for (const auto & oItem : _parent._items) {
            sList += oItem;
            sList += _T('\n');
          }
          auto oDC = wtf::_::device_context::get_client(*this);
          return oDC.get_text_extent(sList);
        }

        void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps) override {
          tstring sList = _T("");
          for (const auto & oItem : _parent._items) {
            sList += oItem;
            sList += _T('\n');
          }
          label::draw_text(dc, ps.client(), sList);
          label::on_wm_paint(dc, ps);
        }


        isa_listbox& _parent;
      }_contents;
    };

  }


  namespace _ {

    template <> struct policy_traits<policy::isa_listbox> {
      using requires = policy_list<policy::isa_scroll_panel>;
    };

  }

  struct listbox : window_impl<listbox, policy::isa_listbox> {
    explicit listbox(window * pParent) : window_impl(pParent) {}
  };

}

#if 0

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct isa_listbox : _SuperT{

      enum class selection_modes{
        single,
        multiple,
      };

      selection_modes selection_mode() const{ return _selection_mode; }
      void selection_mode(selection_modes newval){ _selection_mode = newval; }

      void add_item(const tstring& newval){
        _Items.push_back(newval);
      }

      int visible_list_items() const{ return _visible_list_items; }

    protected:
      explicit isa_listbox(window * pParent) :
        _SuperT(pParent),
        _SelectedItems(1, 0),
        _contents(this){}

      void on_wm_create() override{
        _SuperT::border_style(border_styles::raised);
        _SuperT::auto_draw_text(false);
        _SuperT::on_wm_create();
      };

      void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps) override{
        if (!_Items.size()) return _SuperT::on_wm_paint(dc, ps);
        auto client = ps.client();
        auto oTextSize = dc.get_text_extent(_Items[0]);
        _visible_list_items = 0;
        _ItemRects.clear();
        int listWidth = client.right - right_margin - scroll_width - left_margin;
        for (int i = top_margin; i < client.bottom; i += oTextSize.cy){
          _ItemRects.push_back(rect<coord_frame::client>(left_margin, i, listWidth, std::min(i + oTextSize.cy, client.bottom - bottom_margin)));
        }

        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          for (size_t x = 0; x < _SelectedItems.size(); x++){
            if (_SelectedItems[x] == (_TopIndex + static_cast<int>(i))){
              dc.fill(_ItemRects[i], brush::system_brush(system_colors::highlight));
              break;
            }
          }
          ++_visible_list_items;
          _SuperT::draw_text(dc, _ItemRects[i], _Items[i + _TopIndex].c_str());
        }
        _SuperT::on_wm_paint(dc, ps);
      };

      void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
        if (mouse_buttons::left != m.button) return;
        if (selection_modes::single == _selection_mode){
          _SelectedItems.clear();
        }
        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          if (!_ItemRects[i].is_in(m.position)) continue;
          _SelectedItems.push_back(_TopIndex + static_cast<int>(i));
        }
        _SuperT::invalidate();
        _SuperT::on_wm_click(m);
      };

      static const int scroll_width = 15;
      static const int left_margin = 2;
      static const int right_margin = 2;
      static const int top_margin = 2;
      static const int bottom_margin = 2;

      void invalidate(){ _SuperT::invalidate(); }

    private:
      
      struct contents : panel{
        contents(isa_listbox * pParent) : panel(pParent), _parent(pParent) {}

        isa_listbox * _parent;
      }_contents;
      
      int _TopIndex = 0;
      std::vector<tstring> _Items;
      rect<coord_frame::client>::vector _ItemRects;
      std::vector<int> _SelectedItems;
      selection_modes _selection_mode = selection_modes::single;
      int _visible_list_items = 0;
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_listbox>{
      using requires = policy_list<policy::isa_label, policy::wm_mouse_wheel>;
    };

  }

  struct listbox : window_impl<listbox, policy::isa_listbox>{
    explicit listbox(window * pParent) : window_impl(pParent){}
  };
}
#endif
#if 0
/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{

  namespace policy{
    template <typename _SuperT>
    struct isa_listbox : _SuperT{

      enum class selection_modes{
        single,
        multiple,
      };

      selection_modes selection_mode() const{ return _selection_mode; }
      void selection_mode(selection_modes newval){ _selection_mode = newval; }

      void add_item(const tstring& newval){
        _Items.push_back(newval);
        _vscroll.max(1 + _vscroll.max());
      }

      int visible_list_items() const{ return _visible_list_items; }

    protected:
      explicit isa_listbox(window * pParent) :
        _SuperT(pParent),
        _SelectedItems(1, 0),
        _vscroll(this){}

      void on_wm_create() override{
        _SuperT::border_style(border_styles::raised);
        _SuperT::auto_draw_text(false);
        _SuperT::on_wm_create();
        _vscroll.value(0);
        _vscroll.max(0);
      };

      void on_wm_size(const point<coord_frame::client>& p) override{
        _vscroll.move(p.x - scroll_width - right_margin, top_margin, scroll_width, p.y - top_margin - bottom_margin);
        _vscroll.big_step(_visible_list_items / 2);
        _SuperT::on_wm_size(p);
      };

      void on_wm_paint(const wtf::_::device_context& dc, const wtf::_::paint_struct& ps) override{
        if (!_Items.size()) return _SuperT::on_wm_paint(dc, ps);
        auto client = ps.client();
        auto oTextSize = dc.get_text_extent(_Items[0]);
        _visible_list_items = 0;
        _ItemRects.clear();
        int listWidth = client.right - right_margin - scroll_width - left_margin;
        for (int i = top_margin; i < client.bottom; i += oTextSize.cy){
          _ItemRects.push_back(rect<coord_frame::client>(left_margin, i, listWidth, std::min(i + oTextSize.cy, client.bottom - bottom_margin)));
        }

        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          for (size_t x = 0; x < _SelectedItems.size(); x++){
            if (_SelectedItems[x] == (_TopIndex + static_cast<int>(i))){
              dc.fill(_ItemRects[i], brush::system_brush(system_colors::highlight));
              break;
            }
          }
          ++_visible_list_items;
          _SuperT::draw_text(dc, _ItemRects[i], _Items[i + _TopIndex].c_str());
        }
        _SuperT::on_wm_paint(dc, ps);
      };

      void on_wm_mouse_wheel(int16_t delta, const mouse_msg_param<coord_frame::client>& param) override{
        if (delta > 0) _vscroll.value( _vscroll.value() + 1);
        else _vscroll.value(_vscroll.value() - 1);
        _SuperT::on_wm_mouse_wheel(delta, param);
      };

      void on_wm_click(const mouse_msg_param<coord_frame::client>& m) override{
        if (mouse_buttons::left != m.button) return;
        if (selection_modes::single == _selection_mode){
          _SelectedItems.clear();
        }
        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          if (!_ItemRects[i].is_in(m.position)) continue;
          _SelectedItems.push_back(_TopIndex + static_cast<int>(i));
        }
        _SuperT::invalidate();
        _SuperT::on_wm_click(m);
      };

      static const int scroll_width = 15;
      static const int left_margin = 2;
      static const int right_margin = 2;
      static const int top_margin = 2;
      static const int bottom_margin = 2;

      friend struct vscroll;
      void invalidate(){ _SuperT::invalidate(); }

      class vscroll : public scroll_bar{
        using __super_t = scroll_bar;
      public:

        vscroll(isa_listbox * pParent)
          : __super_t(pParent)
          , _Parent(pParent) {}

        void on_wm_create() override{
          __super_t::orientation(orientations::vertical);
          __super_t::on_wm_create();
        }

        void on_value_changed(int prev_val) override{
          if (prev_val < __super_t::value()){
            for (int i = prev_val; i < __super_t::value(); ++i){
              if ((_Parent->_TopIndex + _Parent->_ItemRects.size()) < _Parent->_Items.size()) _Parent->_TopIndex++;
            }
          } else{
            for (int i = __super_t::value(); i < prev_val; ++i){
              if (_Parent->_TopIndex) --_Parent->_TopIndex;
            }
          }
          _Parent->invalidate();
          __super_t::on_value_changed(prev_val);
        }


        isa_listbox * _Parent;
      };

    private:

      UINT_PTR _MouseDownTimer;
      bool _IncrementHeldDown;
      point<coord_frame::client> _MouseDownAt;
      int _TopIndex = 0;
      std::vector<tstring> _Items;
      rect<coord_frame::client>::vector _ItemRects;
      std::vector<int> _SelectedItems;
      vscroll _vscroll;
      selection_modes _selection_mode = selection_modes::single;
      brush _background_brush = brush::system_brush(system_colors::window);
      int _visible_list_items = 0;
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_listbox>{
      using requires = policy_list<policy::isa_label, policy::isa_scroll_window, policy::wm_mouse_wheel>;
    };

  }

  struct listbox : window_impl<listbox, policy::isa_listbox>{
    explicit listbox(window * pParent) : window_impl(pParent){}
  };
}
#endif