#pragma once
namespace wtf{


    struct tab_container : wtf::window<tab_container, policy::has_size, policy::has_paint, policy::has_show, policy::has_create>
    {

      explicit tab_container(window<void> * pParent)
        : window(pParent), _active_page(0), _button_bar_slider(this), _tab_orientation(tab_orientations::top)
      { }

      virtual void OnCreate() override{
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);
        _button_bar_slider.hide();
      };

      virtual void OnResized(const point::client_coords& newsize) override{
        point::client_coords p = newsize;
        p.x--; p.y--;
        int iBtnPos = _button_left;
        _button_bar_slider.move(p.x - (_tab_height * 2), 0, _tab_height * 2, _tab_height);
        for (auto & oPageInfo : _pages){
          auto PreferedSize = oPageInfo->button()->prefered_text_size();
          PreferedSize.cx += 6;
          if (PreferedSize.cx < _tab_min_width) PreferedSize.cx = _tab_min_width;
          if (PreferedSize.cx > _tab_max_width) PreferedSize.cx = _tab_max_width;
          oPageInfo->button()->move(iBtnPos, 0, PreferedSize.cx, _tab_height);
          iBtnPos += PreferedSize.cx;
          iBtnPos--;
          oPageInfo->panel()->move(0, _tab_height - 1, p.x, p.y - _tab_height);
        }
      };
      
      enum class tab_orientations{
        top,
        left, 
        right, 
        bottom,
      };


      template <typename _Ty>
      panel& add_custom_page(const tstring& title){
        ipage_info::ptr oRet(new page_info<_Ty>(this, title, _pages.size()));
        _pages.push_back(oRet);
        oRet->panel()->border_style(panel::border_styles::raised);
        oRet->button()->border_style(label::border_styles::flat);
        active_page(_active_page);
        return *oRet->panel();
      }

      panel& add_page(const tstring& title){
        return add_custom_page<wtf::panel>(title);
      }

      size_t page_count(){ return _pages.size(); }

      panel& operator[](size_t index){
        assert(index && index < _pages.size());
        return *(_pages[index]->panel());
      }

      const panel& operator[](size_t index) const{
        assert(index && index < _pages.size());
        return *(_pages[index]->panel());
      }

      uint16_t tab_min_width() const{ return _tab_min_width; }
      void tab_min_width(uint16_t newval){ _tab_min_width = newval; }

      uint16_t tab_max_width() const{ return _tab_max_width; }
      void tab_max_width(uint16_t newval){ _tab_max_width = newval; }

    protected:

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
          _pages[i]->button()->zorder(*(_pages[_active_page]->panel()));
        }
        _pages[_active_page]->panel()->zorder(*_pages[_active_page]->button());
      }

      struct ipage_info{
        using ptr = std::shared_ptr<ipage_info>;
        using vector = std::vector<ptr>;

        virtual void activate() = 0;
        virtual void deactivate() = 0;
        virtual wtf::panel * panel() = 0;
        virtual wtf::label * button() = 0;

      };

      template <typename _PanelT=panel>
      struct page_info : ipage_info{
        
        virtual wtf::panel * panel() override{ return &_panel; }
        virtual wtf::label * button() override{ return &_button; }


        tab_container * _parent;
        size_t _PageIndex;

        struct inner_panel : _PanelT{
          inner_panel(tab_container * pParent) : _PanelT(pParent){
            make_window();
          }
        }_panel;

        virtual void deactivate() override{
          _panel.hide();
          _panel.zorder(panel::zorders::bottom);
          _button.deactivate();
        }

        virtual void activate() override{
          _panel.show();
          _button.activate();
          _panel.refresh();
        }

        struct tab_button : label{

          tab_button(tab_container * pParent, size_t PageIndex, const tstring& stitle) : label(pParent), _parent(pParent), _PageIndex(PageIndex), _title(stitle){
            make_window();
          }

          virtual void OnCreate() override{
            text(_title);
            enable_border_elements(true, true, false, true);
            deactivate();
          };

          virtual void OnClick(const policy::mouse_event& m){
            if (policy::mouse_event::buttons::left != m.button) return;
            _parent->active_page(_PageIndex);
          };

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


          size_t _PageIndex;
          tab_container * _parent;
          tstring _title;

        }_button;


        page_info(tab_container * pParent, const tstring& sTitle, size_t PageIndex)
          : _parent(pParent), _PageIndex(PageIndex), _panel(pParent), _button(pParent, PageIndex, sTitle)
        {
        }
      };



      size_t _active_page;
      tab_orientations _tab_orientation;
      ipage_info::vector _pages;      
      scroll_bar _button_bar_slider;
      uint16_t _tab_height = 20;
      uint16_t _tab_min_width = 50;
      uint16_t _tab_max_width = 250;
      int _button_left = 0;
    };


}
