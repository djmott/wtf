#pragma once
namespace wtf{
  namespace default_theme{

    struct tab_container : wtf::window<tab_container, has_size, has_border, has_paint, has_show, has_size>{

      tab_container(HWND parent) : window(parent), 
        _button_bar(*this), _button_bar_slider(*this)
      {
        _button_bar_slider.orientation(scroll_bar::orientations::horizontal);
        ResizedEvent.connect<tab_container, &tab_container::OnResize>(this);
      }

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

      void OnResize(wm_size_flags, int width, int height){}

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

        panel _panel;
        button _button;
      };

      page_info::vector _pages;
      panel _button_bar;
      scroll_bar _button_bar_slider;
      size_t _active_page;
    };

  }
}
