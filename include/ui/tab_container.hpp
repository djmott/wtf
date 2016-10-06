#pragma once
namespace wtf{


    struct tab_container : wtf::window<tab_container, policy::has_size,
      policy::has_border, policy::has_paint, policy::has_show, policy::has_size>
    {

      tab_container() = delete;
      tab_container(const tab_container&) = delete;
      tab_container(tab_container&&) = delete;
      tab_container &operator=(const tab_container &) = delete;
      tab_container &operator=(tab_container&&) = delete;
      virtual ~tab_container() = default;
      explicit tab_container(HWND parent) : window(parent),
        _button_bar(*this), _button_bar_slider(_button_bar), _tab_orientation(tab_orientations::top)
      {
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);
        _button_bar.border_style(panel::border_styles::none);

      }

      enum class tab_orientations{
        top,
        left, 
        right, 
        bottom,
      };

      panel& add_page(const tstring& title){
        _pages.emplace_back(new page_info(*this, title));
        auto oClient = rect::client_coord::get(*this);
        
        ResizedEvent(window::wm_size_flags::restored, point::client_coords(oClient.right, oClient.bottom));
        auto & oPanelInfo = _pages.back();
        oPanelInfo->_panel.border_style(panel::border_styles::none);
        oPanelInfo->_button.border_style(label::border_styles::flat);
        if (1 == _pages.size()) _active_page = 0;
        return oPanelInfo->_panel;
      }

      size_t page_count(){ return _pages.size(); }

      panel& operator[](size_t index){
        assert(index && index < _pages.size());
        return _pages[index]->_panel;
      }
      const panel& operator[](size_t index) const{
        assert(index && index < _pages.size());
        return _pages[index]->_panel;
      }

    protected:

      virtual void ResizedEvent(wm_size_flags, const point::client_coords& newsize) override{
        point::client_coords p = newsize;
        p.x--; p.y--;
        int iBtnPos = 0;
        if (tab_orientations::top == _tab_orientation){
          _button_bar.move(0, 0, p.x, p.y);
          _button_bar_slider.move(p.x - (p.y * 2), 0, p.y * 2, p.y);
          for (auto & oPageInfo : _pages){
            oPageInfo->_button.move(iBtnPos, 0, 50, p.y); iBtnPos += 50;
            oPageInfo->_panel.move(0, p.y, p.x, p.y - p.y);
          }
        } else if (tab_orientations::bottom == _tab_orientation){
          _button_bar.move(0, p.y - p.y, p.x, p.y - p.y);
          _button_bar_slider.move(p.x - (p.y * 2), p.y - p.y, p.y * 2, p.y);
          for (auto & oPageInfo : _pages){
            oPageInfo->_button.move(iBtnPos, p.y-p.y, 50, p.y); iBtnPos += 50;
            oPageInfo->_panel.move(0, 0, p.x, p.y - p.y);
          }
        } else if (tab_orientations::left == _tab_orientation){
          _button_bar.move(p.x - p.y, 0, p.y, p.y);
          _button_bar_slider.move(p.x - p.y, p.y - (p.y * 2), p.y, p.y * 2);
          for (auto & oPageInfo : _pages){
            oPageInfo->_button.move(0, iBtnPos, p.y, 50); iBtnPos += 50;
            oPageInfo->_panel.move(0, 0, p.x - p.y, p.y);
          }
        } else{ //right
          _button_bar.move(0, 0, p.y, p.y);
          _button_bar_slider.move(0, p.y - (p.y * 2), p.y * 2, p.y);
          for (auto & oPageInfo : _pages){
            oPageInfo->_button.move(p.x - p.y, iBtnPos, p.y, 50); iBtnPos += 50;
            oPageInfo->_panel.move(p.y, 0, p.x - p.y, p.y);
          }
        }
      }

      void HidePages(){
        for (auto & oPage : _pages){
          oPage->_panel.hide();
          oPage->_button.border_style(label::border_styles::flat);
        }
      }

      struct page_info{
        using ptr = std::unique_ptr<page_info>;
        using vector = std::vector<ptr>;
        page_info(tab_container& parent, const tstring& sTitle) 
          : _parent(parent), _panel(parent), _button(*this, sTitle){}

        page_info(const page_info&) = delete;
        page_info(page_info&&) = delete;
        page_info &operator=(const page_info &) = delete;
        page_info &operator=(page_info&&) = delete;

        tab_container& _parent;
        panel _panel;

        struct tab_button : label{
          tab_button(page_info& parent, const tstring& sTitle) : label(parent._parent._button_bar), _parent(parent){
            text(sTitle);
            border_style(border_styles::flat);
            enable_border_elements(true, true, false, true);
          }

          virtual void ClickEvent(const point&){
            _parent._parent.HidePages();
            _parent._panel.show();
            border_style(border_styles::double_raised);
          }

          page_info& _parent;

        }_button;
      };

      tab_orientations _tab_orientation;
      page_info::vector _pages;
      panel _button_bar;
      scroll_bar _button_bar_slider;
      size_t _active_page;
      int _tab_height = 15;
    };


}
