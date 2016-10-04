#pragma once

namespace wtf{

    struct textbox : wtf::window<textbox, policy::has_focus, policy::has_cursor, policy::has_caret,
      policy::has_size, policy::has_border, policy::has_click, policy::has_text, policy::has_paint>
    {
      virtual const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

      textbox() = delete;
      textbox(const textbox&) = delete;
      textbox(textbox&&) = delete;
      textbox &operator=(const textbox &) = delete;
      textbox &operator=(textbox&&) = delete;

      explicit textbox(HWND hParent) : window(hParent, true){
        background_brush(brush::system_brush(system_colors::window));
        border_edge(border_edges::thin_sunken);
      }

    private:
    };

}
