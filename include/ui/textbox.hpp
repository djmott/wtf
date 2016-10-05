#pragma once

namespace wtf{

    struct textbox : wtf::window<textbox, policy::has_focus, policy::has_cursor, policy::has_caret, policy::has_keyboard,
      policy::has_size, policy::has_border, policy::has_click, policy::has_text, policy::has_paint, policy::has_mouse>
    {
      virtual const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

      textbox() = delete;
      textbox(const textbox&) = delete;
      textbox(textbox&&) = delete;
      textbox &operator=(const textbox &) = delete;
      textbox &operator=(textbox&&) = delete;

      explicit textbox(HWND hParent) : window(hParent),
        _text(_T("")),
        _edit_pos(0),
        _print_pos(0)
      {
        background_brush(brush::system_brush(system_colors::window));
        border_style(border_styles::lowered);
        text_vertical_alignment(text_vertical_alignments::top);
        text_horizontal_alignment(text_horizontal_alignments::left);
      }
    

      virtual const tstring &text() const{ return _text; }
      virtual void text(const tstring &newval){ _edit_pos = 0; _text = newval; }

    private:

      virtual void PaintEvent(const device_context& dc, const paint_struct& ps) override{
        ApplyFontEvent(dc);

        rect client(ps.rcPaint);
        client.top += border_width();
        client.left += border_width();
        client.bottom -= border_width();
        client.right -= border_width();



        concrete_policy_type<policy::has_text>::text(_text);
        

        DrawText(dc, client);
      }

      virtual void ResizedEvent(wm_size_flags, uint16_t width, uint16_t height) override{
        auto dc = device_context::get_client(*this);
        _text_metrics = text_metrics::get(dc);
      }


      virtual void MouseMoveEvent(event_vkeys, const point& p) override{
      }
      virtual void MouseLButtonDownEvent(event_vkeys, const point& p) override{
        set_focus();
      }
      virtual void MouseLButtonUpEvent(event_vkeys, const point& p) override{
      }
      virtual void MouseWheelEvent(event_vkeys, int16_t /*delta*/, const point& p) override{
      }

      virtual void CharEvent(TCHAR character, key_data) override {
        switch (character){
          case VK_BACK:{
            if (_edit_pos) _text.erase(--_edit_pos,1); break;
          }
          default: {
            _text.insert(_edit_pos++, 1, character);
          }
        }
        refresh();
      }
      virtual void KeyDownEvent(TCHAR key, key_data)override{
        if (key == VK_LEFT){
          std::cout << "Going left" << std::endl;
        } else if (key == VK_RIGHT){
          std::cout << "Going right" << std::endl;
        }
      };
      virtual void KeyUpEvent(TCHAR, key_data)override{};

    private:
      text_metrics _text_metrics;
      tstring::size_type _edit_pos;
      tstring::size_type _print_pos;
      tstring _text;
    };

}
