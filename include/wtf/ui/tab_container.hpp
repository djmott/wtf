#pragma once


namespace wtf{

  template <typename _ImplT, policy..._Policies>
  class window<_ImplT, policy::isa_tab_container, _Policies...> :
    public window<_ImplT, policy::isa_panel, _Policies...>
  {
    using _tab_container_t = window<_ImplT, policy::isa_tab_container, _Policies...>;
    using __super_t = window<_ImplT, policy::isa_panel, _Policies...>;
  public:

    explicit window(iwindow * pParent)
      : __super_t(pParent), _active_page(0), _button_bar_slider(this), _tab_orientation(tab_orientations::top){}
  
 
    enum class tab_orientations{
      top,
      left,
      right,
      bottom,
    };


    template <typename _Ty>
    iwindow& add_custom_page(const tstring& title){
      ipage_info::ptr oRet(new page_info<_Ty>(this, title, _pages.size()));
      _pages.push_back(oRet);
      oRet->panel()->border_style(border_styles::raised);
      oRet->button()->border_style(border_styles::flat);
      active_page(_active_page);
      return *oRet->panel();
    }

    iwindow& add_page(const tstring& title){
      return add_custom_page<wtf::panel>(title);
    }

    size_t page_count(){ return _pages.size(); }

    iwindow& operator[](size_t index){
      assert(index && index < _pages.size());
      return *(_pages[index]->panel());
    }

    const iwindow& operator[](size_t index) const{
      assert(index && index < _pages.size());
      return *(_pages[index]->panel());
    }

    uint16_t tab_min_width() const{ return _tab_min_width; }
    void tab_min_width(uint16_t newval){ _tab_min_width = newval; }

    uint16_t tab_max_width() const{ return _tab_max_width; }
    void tab_max_width(uint16_t newval){ _tab_max_width = newval; }

  protected:

    virtual void on_wm_create() override{
      _button_bar_slider.orientation(orientations::horizontal);
      _button_bar_slider.hide();
      return __super_t::on_wm_create(bHandled);
    };

    virtual void on_wm_size(const point<coord_frame::client>& newsize) override{
      point<coord_frame::client> p = newsize;
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
      return __super_t::on_wm_size(newsize);
    };

    size_t active_page() const{ return _active_page; }
    void active_page(size_t newval){
      _active_page = newval;
      for (size_t i = 0; i < _pages.size(); ++i){
        if (i == _active_page) continue;
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
      virtual wtf::iwindow * panel() = 0;
      virtual wtf::iwindow * button() = 0;

    };

    template <typename _PanelT = panel>
    struct page_info : ipage_info{

      virtual wtf::iwindow * panel() override{ return &_panel; }
      virtual wtf::iwindow * button() override{ return &_button; }


      _tab_container_t * _parent;
      size_t _PageIndex;

      struct inner_panel : _PanelT{
        inner_panel(_tab_container_t * pParent) : _PanelT(pParent){
          exec();
        }
        void invalidate(){ _PanelT::invalidate(true); }
      }_panel;

      virtual void deactivate() override{
        _panel.hide();
        _panel.zorder(panel::zorders::bottom);
        _button.deactivate();
      }

      virtual void activate() override{
        _panel.show();
        _button.activate();
        _panel.invalidate();
      }

      struct tab_button : label{

        tab_button(_tab_container_t * pParent, size_t PageIndex, const tstring& stitle) : label(pParent), _parent(pParent), _PageIndex(PageIndex), _title(stitle){
          exec();
        }

        virtual LRESULT on_wm_create(bool& bCreate) override{
          text(_title);
          enable_border_elements(true, true, false, true);
          deactivate();
          return label::on_wm_create(bCreate);
        };

        virtual void on_wm_click(const mouse_msg_param& m) override{
          if (mouse_msg_param::buttons::left == m.button) _parent->active_page(_PageIndex);
          label::on_wm_click(m);
        };

        void deactivate(){
          border_style(border_styles::flat);
          fore_color(wtf::system_rgb<system_colors::gray_text>());
          font().weight(font::weights::normal);
          zorder(zorders::bottom);
          invalidate();
        }

        void activate(){
          border_style(border_styles::raised);
          fore_color(wtf::system_rgb<system_colors::button_text>());
          font().weight(font::weights::bold);
          invalidate();
        }


        size_t _PageIndex;
        _tab_container_t * _parent;
        tstring _title;

      }_button;


      page_info(_tab_container_t * pParent, const tstring& sTitle, size_t PageIndex)
        : _parent(pParent), _PageIndex(PageIndex), _panel(pParent), _button(pParent, PageIndex, sTitle){}
    };

    struct scrollbar_t : window<scrollbar_t, policy::isa_scrollbar>{
      scrollbar_t(iwindow*pParent): window(pParent){}
    };

    size_t _active_page;
    tab_orientations _tab_orientation;
    typename ipage_info::vector _pages;
    scrollbar_t _button_bar_slider;
    uint16_t _tab_height = 20;
    uint16_t _tab_min_width = 50;
    uint16_t _tab_max_width = 250;
    int _button_left = 0;
  };


}
