#pragma once
namespace wtf{


    struct tab_container : wtf::window<tab_container, policy::has_size, policy::has_paint, policy::has_show>
    {
      tab_container() = delete;
      tab_container(const tab_container&) = delete;
      tab_container(tab_container&&) = delete;
      tab_container &operator=(const tab_container &) = delete;
      tab_container &operator=(tab_container&&) = delete;
      virtual ~tab_container() = default;
      explicit tab_container(HWND parent) 
        : window(parent),
        _button_bar_slider(*this), 
        _tab_orientation(tab_orientations::top)
      {        
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);
      }

      enum class tab_orientations{
        top,
        left, 
        right, 
        bottom,
      };

      panel& add_page(const tstring& title){
        _pages.emplace_back(new page_info(*this, title));
        ResizedEvent(window::wm_size_flags::restored, rect::client_coord::get(*this).dimensions());
        auto & oPanelInfo = _pages.back();
        oPanelInfo->_panel.border_style(panel::border_styles::raised);
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
        int iBtnPos = _button_left;
        _button_bar_slider.move(p.x - (_tab_height * 2), 0, _tab_height * 2, _tab_height);
        for (auto & oPageInfo : _pages){
          oPageInfo->_button.move(iBtnPos, 0, _tab_width, _tab_height); 
          iBtnPos += _tab_width;
          iBtnPos--;
          oPageInfo->_panel.move(0, _tab_height, p.x, p.y - _tab_height);
        }
      }

      void HidePages(){
        for (auto & oPage : _pages){
          oPage->_panel.hide();
          oPage->_button.border_style(label::border_styles::flat);
          oPage->_button.font().weight(font::weights::normal);
          oPage->_button.refresh();
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
          tab_button(page_info& parent, const tstring& sTitle) : label(parent._parent), _parent(parent){
            text(sTitle);
            border_style(border_styles::flat);
            enable_border_elements(true, true, false, true);
          }

          virtual void ClickEvent(const point::client_coords&) override{
            _parent._parent.HidePages();
            _parent._panel.show();
            border_style(border_styles::double_raised);
            font().weight(font::weights::bold);
            refresh();
          }

          page_info& _parent;

        }_button;


      };



      tab_orientations _tab_orientation;
      page_info::vector _pages;      
      scroll_bar _button_bar_slider;
      size_t _active_page;
      int _tab_height = 15;
      int _tab_width = 50;
      int _button_left = 0;
    };


}
