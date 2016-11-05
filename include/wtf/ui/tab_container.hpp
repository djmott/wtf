/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once


namespace wtf{

  enum class tab_orientations{
    top,
    left,
    right,
    bottom,
  };

  namespace policy{

    template <typename _SuperT>
    struct isa_tab_page :  _SuperT{

    protected:
      template <typename> friend struct isa_tab_container;
      isa_tab_page(window * pParent) : _SuperT(pParent){}
    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_tab_page>{
      using requires = policy_list<policy::isa_panel>;
    };
  }

  class tab_page : public window_impl<tab_page, policy::isa_tab_page>{
    template <typename, template <typename> class...> friend class window_impl;
   public:
    tab_page(window * pParent) : window_impl(pParent){}
  };

  namespace policy{

    template <typename _SuperT>
    struct isa_tab_container : _SuperT{

      explicit isa_tab_container(window * pParent) : _SuperT(pParent), _button_bar_slider(this){
        _SuperT::border_style(border_styles::none);
      }

      template <typename _Ty>
      tab_page& add_custom(const tstring& title){
        typename ipage_info::ptr oRet(new page_info<_Ty>(this, title, _pages.size()));
        _pages.push_back(oRet);
        oRet->page().border_style(border_styles::raised);
        oRet->button.border_style(border_styles::flat);
        if (_SuperT::_handle) {
          oRet->exec();
          active_page(_active_page);
        }
        return oRet->page();
      }


      tab_page& add(const tstring& title){
        return add_custom<tab_page>(title);
      }

      size_t page_count(){ return _pages.size(); }

      tab_page& operator[](size_t index){
        assert(index && index < _pages.size());
        return *(_pages[index]->page());
      }

      const tab_page& operator[](size_t index) const{
        assert(index && index < _pages.size());
        return *(_pages[index]->page());
      }

      uint16_t tab_min_width() const{ return _tab_min_width; }
      void tab_min_width(uint16_t newval){ _tab_min_width = newval; }

      uint16_t tab_max_width() const{ return _tab_max_width; }
      void tab_max_width(uint16_t newval){ _tab_max_width = newval; }

    protected:

      
      int exec() override{
        auto iRet = _SuperT::exec();
        for (auto & oPage : _pages){
          oPage->exec();
        }
        return iRet;
      }

      void on_wm_create() override{
        _button_bar_slider.orientation(orientations::horizontal);
        _button_bar_slider.hide();
        _SuperT::on_wm_create();
      };

      void on_wm_size(const point<coord_frame::client>& newsize) override{
        point<coord_frame::client> p = newsize;
        int iBtnPos = _button_left;
        _button_bar_slider.move(p.x - (_tab_height * 2), 0, _tab_height * 2, _tab_height);
        for (auto & oPageInfo : _pages){
          auto PreferedSize = oPageInfo->button.prefered_text_size();
          PreferedSize.cx += 6;
          if (PreferedSize.cx < _tab_min_width) PreferedSize.cx = _tab_min_width;
          if (PreferedSize.cx > _tab_max_width) PreferedSize.cx = _tab_max_width;
          oPageInfo->button.move(iBtnPos, 0, PreferedSize.cx, 1 + _tab_height);
          iBtnPos += PreferedSize.cx;
          iBtnPos--;
          oPageInfo->page().move(0, _tab_height - 1, p.x, p.y - _tab_height);
        }
        _SuperT::on_wm_size(newsize);
      };

      size_t active_page() const{ return _active_page; }
      void active_page(size_t newval){
        _active_page = newval;
        for (size_t i = 0; i < _pages.size(); ++i){
          if (i == _active_page) continue;
          _pages[i]->deactivate();
        }
        _pages[newval]->activate();
      }

      struct ipage_info{
        using ptr = std::shared_ptr<ipage_info>;
        using vector = std::vector<ptr>;

        isa_tab_container * _parent;
        size_t _PageIndex;

        virtual tab_page& page() = 0;
        virtual const tab_page& page() const = 0;

        virtual void exec() {
          button.exec();
        }

        class button : public window_impl<button, policy::isa_button>{
          using __super_t = window_impl<button, policy::isa_button>;
          template <typename> friend struct isa_tab_container;
        public:

          button(isa_tab_container * pParent, size_t PageIndex, const tstring& stitle)
            : __super_t(pParent), _parent(pParent), _PageIndex(PageIndex), _title(stitle)
          {
            __super_t::text(_title);
            __super_t::enable_border_elements(true, true, false, true);
          }

          void on_wm_create() override{
            deactivate();
            __super_t::on_wm_create();
          };

          void on_wm_click(const mouse_msg_param& m) override{
            if (mouse_msg_param::buttons::left == m.button) _parent->active_page(_PageIndex);
            __super_t::on_wm_click(m);
          };

          void deactivate(){
            __super_t::border_style(border_styles::flat);
            __super_t::fore_color(wtf::system_rgb<system_colors::gray_text>());
            __super_t::font().weight(font::weights::normal);
            __super_t::zorder(zorders::bottom);
          }

          void activate(){
            __super_t::border_style(border_styles::raised);
            __super_t::fore_color(wtf::system_rgb<system_colors::button_text>());
            __super_t::font().weight(font::weights::bold);
            __super_t::zorder(zorders::top_most);
          }


          size_t _PageIndex;
          isa_tab_container * _parent;
          tstring _title;

        }button;


        void deactivate(){
          page().hide();
          page().zorder(zorders::bottom);
          button.deactivate();
        }

        void activate(){
          page().show();
          button.activate();
          page().zorder(button);
          page().invalidate();
        }

        ipage_info(isa_tab_container * pParent, const tstring& sTitle, size_t PageIndex)
          : _parent(pParent), _PageIndex(PageIndex), button(pParent, PageIndex, sTitle){}
      };

      template <typename _Ty>
      struct page_info : ipage_info{
        template <typename> friend struct isa_tab_container;

        _Ty _page;

        tab_page& page() override{ return _page; }
        const tab_page& page() const override{ return _page; }

        void exec() override {
          _page.exec();
          ipage_info::exec();
        }

        page_info(isa_tab_container * pParent, const tstring& sTitle, size_t PageIndex)
          : ipage_info(pParent, sTitle, PageIndex), _page(pParent){}

      };


      class scrollbar_t : public window_impl<scrollbar_t, policy::isa_scroll_bar>{
        using __super_t = window_impl<scrollbar_t, policy::isa_scroll_bar>;
      public:
        scrollbar_t(window*pParent) : __super_t(pParent) {}
      };

      size_t _active_page = 0;
      tab_orientations _tab_orientation = tab_orientations::top;
      typename ipage_info::vector _pages;
      scrollbar_t _button_bar_slider;
      uint16_t _tab_height = 20;
      uint16_t _tab_min_width = 50;
      uint16_t _tab_max_width = 250;
      int _button_left = 0;
    };
  }

  namespace _{

    template <> struct policy_traits<policy::isa_tab_container>{
      using requires = policy_list<policy::isa_panel>;
    };

  }


  struct tab_container : window_impl<tab_container, policy::isa_tab_container>{
    explicit tab_container(window * pParent) : window_impl(pParent){}
  };

}
