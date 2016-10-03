#pragma once

namespace wtf{
  namespace default_theme{
    struct textbox : wtf::window<textbox, has_focus, has_cursor, has_caret, has_size, has_border, has_click, has_text, has_paint>{
      virtual const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

      textbox(HWND hParent) : window(hParent, true){
        background_brush(brush::system_brush(system_colors::window));
        border_edge(border_edges::thin_sunken);
      }

    private:
    };
  }
}
