#pragma once
namespace wtf{
  namespace default_theme{

      struct listbox : wtf::window<listbox, has_move, has_background, has_border, has_click_event, has_paint_event, has_font>{
        using vector_type = std::vector<tstring>;

        listbox(HWND hParent) : window(hParent, true), _background_brush(brush::system_brush(system_colors::window)), _items(), _selected_index(-1), _top_index(0), _vscroll(*this){}

        vector_type& items(){ return _items; }
        const vector_type& items() const { return _items; }

        vector_type::size_type selected_index() const{ return _selected_index; }

      

      private:

        virtual const brush& background_brush() const override{ return _background_brush; }

        brush _background_brush;

        vector_type _items;
        vector_type::size_type _selected_index;
        vector_type::size_type _top_index;
        scroll_bar<orientation::vertical> _vscroll;
      };
    }

}