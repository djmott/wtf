#pragma once
namespace wtf{


    struct tab_container : wtf::window<tab_container, policy::has_size,
      policy::has_border, policy::has_paint, policy::has_show, policy::has_size>
    {

      explicit tab_container(HWND parent) : window(parent), 
        _button_bar(*this), _button_bar_slider(*this)
      {
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);

      }
      tab_container() = delete;
      tab_container(const tab_container&) = delete;
      tab_container(tab_container&&) = delete;
      tab_container &operator=(const tab_container &) = delete;
      tab_container &operator=(tab_container&&) = delete;


      panel& add_page(const tstring& title){
        _pages.emplace_back(new page_info(*this, _button_bar, title));
        refresh();
        return _pages.back()->_panel;
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

      virtual void ResizedEvent(wm_size_flags, uint16_t /*width*/, uint16_t /*height*/) override {}

      void HidePages(){
        for (auto & oPage : _pages){
          oPage->_panel.hide();
        }
      }

      struct page_info{
        using ptr = std::unique_ptr<page_info>;
        using vector = std::vector<ptr>;
        page_info(HWND tab, HWND button_bar, tstring sTitle) : 
          _panel(tab), _button(button_bar)
        {
          _button.text(sTitle);
        }

        page_info(const page_info&) = delete;
        page_info(page_info&&) = delete;
        page_info &operator=(const page_info &) = delete;
        page_info &operator=(page_info&&) = delete;

        panel _panel;
        toggle_button _button;
      };

      page_info::vector _pages;
      panel _button_bar;
      scroll_bar _button_bar_slider;
      size_t _active_page;
    };


}
