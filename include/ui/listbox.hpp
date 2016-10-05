#pragma once
namespace wtf{

    struct listbox : wtf::window<listbox, policy::has_border, policy::has_click, policy::has_text,
      policy::has_paint, policy::has_size, policy::has_timer, policy::has_mouse, policy::has_font>
    {
      static const int scroll_width = 15;
      static const int left_margin = 2;
      static const int right_margin = 2;
      static const int top_margin = 2;
      static const int bottom_margin = 2;

      enum class selection_modes{
        single,
        multiple,
      };

      selection_modes selection_mode() const{ return _selection_mode; }
      void selection_mode(selection_modes newval){ _selection_mode = newval; }

      void add_item(const tstring& newval){
        _Items.push_back(newval);
      }
      listbox(const listbox&) = delete;
      listbox(listbox&&) = delete;
      listbox &operator=(const listbox &) = delete;
      listbox &operator=(listbox&&) = delete;

      explicit listbox(HWND hParent) : 
        window(hParent), 
        _TopIndex(0), 
        _SelectedItems(1, 0),
        _vscroll(*this),
        _selection_mode(selection_modes::single),
        _background_brush(brush::system_brush(system_colors::window))
      {
        border_style(border_styles::raised);
        _vscroll.border_style(vscroll::border_styles::none);
      }

    protected:
      friend struct vscroll;

      virtual void ResizedEvent(wm_size_flags, uint16_t width, uint16_t height) override {
        _vscroll.move(width - scroll_width - right_margin, top_margin, scroll_width, height - top_margin - bottom_margin);
      };

      virtual const brush &background_brush() const{ return _background_brush; }

      virtual void PaintEvent(const device_context& dc, const paint_struct& ps) override {
        if (!_Items.size()) return;
        ApplyFontEvent(dc);
        rect client(ps.rcPaint);
        auto oTextSize = dc.get_text_extent(_Items[0]);
        _ItemRects.clear();
        int listWidth = client.right - right_margin - scroll_width - left_margin;
        for (int i = top_margin; i < client.bottom; i += oTextSize.cy){
          _ItemRects.push_back(rect(left_margin, i, listWidth, std::min(i + oTextSize.cy, client.bottom - bottom_margin)));
        }

        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          for (size_t x = 0; x < _SelectedItems.size(); x++){
            if (_SelectedItems[x] == (_TopIndex + static_cast<int>(i))){
              dc.fill(_ItemRects[i], brush::system_brush(system_colors::highlight));
              break;
            }
          }
          text(_Items[i + _TopIndex]);
          DrawText(dc, _ItemRects[i]);
        }
      }

      struct vscroll : scroll_bar{
        vscroll(listbox& parent) : scroll_bar(parent), _Parent(parent){
          orientation(scroll_bar::orientations::vertical);
        }

        virtual void IncrementEvent(){
          if ((_Parent._TopIndex + _Parent._ItemRects.size()) < _Parent._Items.size()) _Parent._TopIndex++;
          _Parent.refresh(true);
        }

        virtual void DecrementEvent(){
          if (!_Parent._TopIndex) return;
          --_Parent._TopIndex;
          _Parent.refresh(true);
        }
        listbox& _Parent;
      };

      virtual void MouseWheelEvent(event_vkeys, int16_t delta, const point&p) override{
        if (delta > 0) _vscroll.DecrementEvent();
        else _vscroll.IncrementEvent();
      }


      virtual void ClickEvent(const point& p) override {
        if (selection_modes::single == _selection_mode){
          _SelectedItems.clear();
        }
        for (size_t i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
          if (!_ItemRects[i].is_in(p)) continue;
          _SelectedItems.push_back(_TopIndex + static_cast<int>(i));
        }
        refresh(true);
      }

      private:
        virtual const tstring &text() const{ return concrete_policy_type<policy::has_text>::text(); }
        virtual void text(const tstring &newval){ concrete_policy_type<policy::has_text>::text(newval); }


      UINT_PTR _MouseDownTimer;
      bool _IncrementHeldDown;
      point _MouseDownAt;
      int _TopIndex;
      std::vector<tstring> _Items;
      std::vector<rect> _ItemRects;
      std::vector<int> _SelectedItems;
      vscroll _vscroll;
      selection_modes _selection_mode;
      brush _background_brush;
    };

}
