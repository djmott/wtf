#pragma once
namespace wtf{
  namespace default_theme{

      struct listbox : wtf::window<listbox, has_background, has_border, has_click, has_paint, has_font, has_size, has_timer, has_mouse_events>{

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


        listbox(HWND hParent) : window(hParent), _TopIndex(0), _vscroll(*this), _selection_mode(selection_modes::single),
          _SelectedItems(1, 0)        
        {
          background_brush(brush::system_brush(system_colors::window));
          border_edge(border_edges::raised);
          ResizedEvent.connect([this](wm_size_flags, int width, int height){
            _vscroll.move(width - scroll_width - right_margin, top_margin, scroll_width, height-top_margin-bottom_margin);
          });
          PaintEvent.connect([this](const device_context& oDC, const paint_struct& oPS){
            auto flags = device_context::draw_text_flags::left;
            if (!_Items.size()) return;
            auto oClientArea = oPS.client_area();
            auto oTextSize = oDC.get_text_extent(_Items[0]);
            _ItemRects.clear();
            int listWidth = oClientArea.right - right_margin - scroll_width - left_margin;
            for (int i = top_margin; i < oClientArea.bottom; i += oTextSize.cy){
              _ItemRects.push_back(rect(left_margin, i, listWidth, std::min(i + oTextSize.cy, oClientArea.bottom-bottom_margin)));
            }

            for (int i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
              oDC.background_mix_mode(device_context::background_mix_modes::opaque);
              for (int x = 0; x < _SelectedItems.size(); x++){
                if (_SelectedItems[x] == (_TopIndex + i)){
                  oDC.fill(_ItemRects[i], brush::system_brush(system_colors::highlight));
                  oDC.background_mix_mode(device_context::background_mix_modes::transparent);
                  break;
                }
              }
              oDC.draw_text(_Items[i + _TopIndex], _ItemRects[i], flags);
            }
          });
          _vscroll.increment_button.ClickEvent.connect([this](const point&){
            if ((_TopIndex + _ItemRects.size()) < _Items.size()) _TopIndex++;
            refresh(true);
          });
          _vscroll.decrement_button.ClickEvent.connect([this](const point&){ 
            if (!_TopIndex) return;
            --_TopIndex;
            refresh(true);
          });
          ClickEvent.connect([this](const point& p){
            if (selection_modes::single == _selection_mode){
              _SelectedItems.clear();
            }
            for (int i = 0; i < _ItemRects.size() && (_TopIndex + i) < _Items.size(); ++i){
              if (!_ItemRects[i].is_in(p)) continue;
              _SelectedItems.push_back(_TopIndex + i);
            }
            refresh(true);
          });
          TimerEvent.connect([this](UINT_PTR iTimer){
            if (iTimer != _MouseDownTimer) return;
            kill_timer(_MouseDownTimer);
            _MouseDownTimer = set_timer(10);
            if (_IncrementHeldDown){
              _vscroll.increment_button.ClickEvent(_MouseDownAt);
            } else{
              _vscroll.decrement_button.ClickEvent(_MouseDownAt);
            }
          });
          _vscroll.increment_button.MouseLButtonDownEvent.connect([this](event_vkeys, const point& p){
            _MouseDownAt = p;
            _IncrementHeldDown = true;
            _MouseDownTimer = set_timer(500);
          });
          _vscroll.increment_button.MouseLButtonUpEvent.connect([this](event_vkeys, const point& p){
            kill_timer(_MouseDownTimer);
          });
          _vscroll.decrement_button.MouseLButtonDownEvent.connect([this](event_vkeys, const point& p){
            _MouseDownAt = p;
            _IncrementHeldDown = false;
            _MouseDownTimer = set_timer(500);
          });
          _vscroll.decrement_button.MouseLButtonUpEvent.connect([this](event_vkeys, const point& p){
            kill_timer(_MouseDownTimer);
          });
          MouseWheelEvent.connect([this](event_vkeys, short delta, const point& p){
            if (delta < 0){
              _vscroll.increment_button.ClickEvent(p);
            } else{
              _vscroll.decrement_button.ClickEvent(p);
            }

          });
        }
        

      protected:
        UINT_PTR _MouseDownTimer;
        bool _IncrementHeldDown;
        point _MouseDownAt;
        int _TopIndex;
        std::vector<tstring> _Items;
        std::vector<rect> _ItemRects;
        std::vector<int> _SelectedItems;
        scroll_bar<orientation::vertical> _vscroll;
        selection_modes _selection_mode;
      };
    }

}