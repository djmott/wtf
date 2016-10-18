#pragma once
namespace wtf{


    struct tab_container : wtf::window<tab_container, policy::has_size, policy::has_paint, policy::has_show, policy::has_create>
    {

      explicit tab_container(iwindow * pParent)
        : window(pParent), _active_page(0), _button_bar_slider(this), _tab_orientation(tab_orientations::top)
      {        
      }

      enum class tab_orientations{
        top,
        left, 
        right, 
        bottom,
      };


      template <typename _Ty>
      panel& add_custom_page(const tstring& title){
        _pages.push_back(page_info::make<_Ty>(this, title, _pages.size()));
        ResizedEvent(window::wm_size_flags::restored, rect::client_coord::get(*this).dimensions());
        auto & oPanelInfo = _pages.back();
        oPanelInfo->_panel->border_style(panel::border_styles::raised);
        oPanelInfo->_button.border_style(label::border_styles::flat);
        active_page(_active_page);
        return *oPanelInfo->_panel;
      }

      panel& add_page(const tstring& title){
        return add_custom_page<panel>(title);
      }

      size_t page_count(){ return _pages.size(); }

      panel& operator[](size_t index){
        assert(index && index < _pages.size());
        return *(_pages[index]->_panel);
      }

      const panel& operator[](size_t index) const{
        assert(index && index < _pages.size());
        return *(_pages[index]->_panel);
      }

      uint16_t tab_min_width() const{ return _tab_min_width; }
      void tab_min_width(uint16_t newval){ _tab_min_width = newval; }
      uint16_t tab_max_width() const{ return _tab_max_width; }
      void tab_max_width(uint16_t newval){ _tab_max_width = newval; }

      callback<void(const size&)> OnPageResize;

    protected:

      virtual void CreateEvent() override{
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);
        _button_bar_slider.hide();
      }

      virtual void ResizedEvent(wm_size_flags, const point::client_coords& newsize) override{
        point::client_coords p = newsize;
        p.x--; p.y--;
        int iBtnPos = _button_left;
        _button_bar_slider.move(p.x - (_tab_height * 2), 0, _tab_height * 2, _tab_height);
        for (auto & oPageInfo : _pages){
          auto PreferedSize = oPageInfo->_button.prefered_text_size();
          PreferedSize.cx += 6;
          if (PreferedSize.cx < _tab_min_width) PreferedSize.cx = _tab_min_width;
          if (PreferedSize.cx > _tab_max_width) PreferedSize.cx = _tab_max_width;
          oPageInfo->_button.move(iBtnPos, 0, PreferedSize.cx, _tab_height); 
          iBtnPos += PreferedSize.cx;
          iBtnPos--;
          oPageInfo->_panel->move(0, _tab_height-1, p.x, p.y - _tab_height);
        }
        OnPageResize(size(p.x, p.y - _tab_height));
      }

      size_t active_page() const{ return _active_page; }
      void active_page(size_t newval) { 
        _active_page = newval;
        for (size_t i=0 ; i<_pages.size() ; ++i){
          if (i==_active_page) continue;
          _pages[i]->deactivate();
        }
        _pages[newval]->activate();
        for (size_t i = 0; i < _pages.size(); ++i){
          if (i == _active_page) continue;
          _pages[i]->_button.zorder(*(_pages[_active_page]->_panel));
        }
        _pages[_active_page]->_panel->zorder(_pages[_active_page]->_button);
      }

      struct page_info{
        using ptr = std::unique_ptr<page_info>;
        using vector = std::vector<ptr>;
        
        template <typename _Ty>
        static ptr make(tab_container * pParent, const tstring& sTitle, size_t PageIndex){
          ptr oRet(new page_info(pParent, sTitle, PageIndex));
          oRet->_panel.reset(new _Ty(pParent));
          return oRet;
        }



        page_info(const page_info&) = delete;
        page_info(page_info&&) = delete;
        page_info &operator=(const page_info &) = delete;
        page_info &operator=(page_info&&) = delete;

        size_t _PageIndex;
        tab_container * _parent;
        std::unique_ptr<panel> _panel;

        void deactivate(){
          _panel->hide();
          _panel->zorder(panel::zorders::bottom);
          _button.deactivate();
        }

        void activate(){
          _panel->show();
          _button.activate();
          _panel->refresh();
        }



        struct tab_button : label{

          tab_button(page_info * pParent, const tstring& sTitle) : label(pParent->_parent), _parent(pParent){
            text(sTitle);
            enable_border_elements(true, true, false, true);
            deactivate();
          }

          void deactivate(){
            border_style(label::border_styles::flat);
            fore_color(wtf::system_rgb<system_colors::gray_text>());
            font().weight(font::weights::normal);
            zorder(zorders::bottom);
            refresh();
          }

          void activate(){
            border_style(border_styles::raised);
            fore_color(wtf::system_rgb<system_colors::button_text>());
            font().weight(font::weights::bold);
            refresh();
          }

          virtual void ClickEvent(const policy::mouse_event& m) override{
            if (policy::mouse_event::buttons::left != m.button) return;
            _parent->_parent->active_page(_parent->_PageIndex);
          }

          page_info * _parent;

        }_button;



      private:
        page_info(tab_container * pParent, const tstring& sTitle, size_t PageIndex)
          : _parent(pParent), _PageIndex(PageIndex), _button(this, sTitle){}

      };



      size_t _active_page;
      tab_orientations _tab_orientation;
      page_info::vector _pages;      
      scroll_bar _button_bar_slider;
      uint16_t _tab_height = 20;
      uint16_t _tab_min_width = 50;
      uint16_t _tab_max_width = 250;
      int _button_left = 0;
    };


}
