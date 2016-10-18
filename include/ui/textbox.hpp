#pragma once

namespace wtf{

    struct textbox : wtf::window<textbox, policy::has_focus, policy::has_cursor, policy::has_caret, policy::has_keyboard,
      policy::has_size, policy::has_border, policy::has_click, policy::has_text, policy::has_paint, policy::has_mouse_down>
    {

      explicit textbox(iwindow * pParent) : window(pParent){
        background_brush(brush::system_brush(system_colors::window));
        border_style(border_styles::lowered);
        text_vertical_alignment(text_vertical_alignments::top);
        text_horizontal_alignment(text_horizontal_alignments::left);
        caret_width(2);
        auto_draw_text(false);
      }
    
      virtual const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

      virtual const tstring &text() const{ return _text; }
      virtual void text(const tstring &newval){ _edit_pos = 0; _text = newval; }


    private:

      virtual void PaintEvent(const device_context& dc, const paint_struct& ps) override{
        if (!_text.size()) return;

        rect::client_coord client = ps.client();
        client.top += border_width();
        client.left += border_width();
        client.bottom -= border_width();
        client.right -= border_width();
        auto ClientWidth = client.right - client.left;

        //sanity checks
        if (_print_pos < 0) _print_pos = 0;
        if (_edit_pos < 0) _edit_pos = 0;
        if (_edit_pos > static_cast<int>(_text.size())) _edit_pos = static_cast<int>(_text.size());
        if (_print_pos >= _edit_pos && _edit_pos) _print_pos = _edit_pos - 1;


        int EndPrintPos = _edit_pos;
        point::client_coords CaretPos;

        for (;;){
          auto tmpExt = dc.get_text_extent(_text.c_str() + _print_pos, EndPrintPos - _print_pos);
          CaretPos.x = tmpExt.cx;
          CaretPos.y = _text_metrics.tmHeight;
          if (tmpExt.cx <= ClientWidth) break;
          _print_pos++;
        }


        for(;;){
          auto tmpEndPos = 1 + EndPrintPos;
          if (tmpEndPos >= _text.size()) break;
          auto tmpExt = dc.get_text_extent(_text.c_str() + _print_pos, tmpEndPos - _print_pos);
          if (tmpExt.cx > ClientWidth) break;
          EndPrintPos = tmpEndPos;
        }
        
        concrete_policy_type<policy::has_text>::text(_text.substr(_print_pos, EndPrintPos -_print_pos));

        bool bCaretVisible = caret_visible();
        if (bCaretVisible) caret_visible(false);

        CaretPos.y = border_width();

        draw_text(dc, client);

        if (bCaretVisible){
          caret_visible(true);
          caret_position(CaretPos);
        }
      }


      virtual void MouseDownEvent(const policy::mouse_event& p) override{
        set_focus();
      }
      
      virtual void CharEvent(TCHAR character, key_data) override {
        switch (character){
          case VK_BACK:{
            if (_edit_pos) _text.erase(--_edit_pos,1); break;
          }
          default: {
            _text.insert(_edit_pos++, 1, character); break;
          }
        }
//         refresh();
      }

      virtual void KeyDownEvent(TCHAR key, key_data)override{

        switch (key){
          case VK_LEFT:{
            _edit_pos--; break;
          } 
          case VK_RIGHT:{
            _edit_pos++; break;
          }
          case VK_HOME:{
            _edit_pos = _print_pos = 0; break;
          }
          case VK_END:{
            _edit_pos = static_cast<int>(_text.size()); break;
          }
        }
        if (_edit_pos < 0) _edit_pos = 0;
        if (_edit_pos > static_cast<int>(_text.size())) _edit_pos = static_cast<int>(_text.size());
        refresh();
      };

      virtual void KeyUpEvent(TCHAR, key_data)override{};

      virtual void ApplyFontEvent(const device_context& dc) override{
        concrete_policy_type<policy::has_font>::ApplyFontEvent(dc);
        _text_metrics = text_metrics::get(dc);
        caret_height(_text_metrics.tmHeight);
      }
    private:

      text_metrics _text_metrics;
      // _edit_pos tracks the index into _text for the edit cursor 
      int _edit_pos = 0;
      // _print_pos helps ensure the cursor is always visible e.g. text length > window length
      int _print_pos = 0;
      tstring _text = _T("");
      bool _word_wrap = false;
      std::map<TCHAR, uint8_t> _CharWidths;
    };

}
