#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT, typename _ImplT>
    class isa_textbox : public _SuperT{

      
    public:

    protected:
      
      explicit isa_textbox(iwindow * pParent) : _SuperT(pParent){}

      void on_wm_create() override{
	      _SuperT::background_brush(brush::system_brush(system_colors::window));
	      _SuperT::border_style(border_styles::lowered);
	      _SuperT::text_vertical_alignment(text_vertical_alignments::top);
	      _SuperT::text_horizontal_alignment(text_horizontal_alignments::left);
	      _SuperT::caret_width(2);
	      _SuperT::auto_draw_text(false);
        _SuperT::on_wm_create();
      };

      void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
	      if (!_SuperT::_text.size()) return _SuperT::on_wm_paint(dc, ps);

        rect<coord_frame::client> client = ps.client();
        auto ClientWidth = client.right - client.left;

        //sanity checks
        if (_print_pos < 0) _print_pos = 0;
        if (_edit_pos < 0) _edit_pos = 0;
	      if (_edit_pos > static_cast<int>(_SuperT::_text.size())) _edit_pos = static_cast<int>(_SuperT::_text.size());
        if (_print_pos >= _edit_pos && _edit_pos) _print_pos = _edit_pos - 1;


        int EndPrintPos = _edit_pos;
        point<coord_frame::client> CaretPos;

        for (;;){
	        auto tmpExt = dc.get_text_extent(_SuperT::_text.c_str() + _print_pos, EndPrintPos - _print_pos);
          CaretPos.x = tmpExt.cx;
          CaretPos.y = tmpExt.cy;
          if (tmpExt.cx <= ClientWidth) break;
          _print_pos++;
        }


        for (;;){
          auto tmpEndPos = 1 + EndPrintPos;
	        if (tmpEndPos >= _SuperT::_text.size()) break;
	        auto tmpExt = dc.get_text_extent(_SuperT::_text.c_str() + _print_pos, tmpEndPos - _print_pos);
          if (tmpExt.cx > ClientWidth) break;
          EndPrintPos = tmpEndPos;
        }

	      _SuperT::text(_SuperT::_text.substr(_print_pos, EndPrintPos - _print_pos));

	      bool bCaretVisible = _SuperT::caret_visible();
	      if (bCaretVisible) _SuperT::caret_visible(false);

	      CaretPos.y = _SuperT::border_width();

	      _SuperT::draw_text(dc, client);

        if (bCaretVisible){
	        _SuperT::caret_visible(true);
	        _SuperT::caret_position(CaretPos);
        }
        _SuperT::on_wm_paint(dc, ps);
      };


      void on_wm_mouse_up(const mouse_msg_param& p) override{
	      _SuperT::set_focus();
        _SuperT::on_wm_mouse_up(p);
      };


      void on_wm_char(UINT character, keyboard_msg_param k) override{
        switch (character){
          case VK_BACK:
          {
	          if (_edit_pos) _SuperT::_text.erase(--_edit_pos, 1); break;
          }
          default:
          {
	          _SuperT::_text.insert(_edit_pos++, 1, static_cast<char>(character)); break;
          }
        }
	      _SuperT::invalidate();
        _SuperT::on_wm_char(character, k);
      };

      void on_wm_keydown(UINT key, keyboard_msg_param k) override{

        switch (key){
          case VK_LEFT:
          {
            _edit_pos--; break;
          }
          case VK_RIGHT:
          {
            _edit_pos++; break;
          }
          case VK_HOME:
          {
            _edit_pos = _print_pos = 0; break;
          }
          case VK_END:
          {
	          _edit_pos = static_cast<int>(_SuperT::_text.size()); break;
          }
        }
        if (_edit_pos < 0) _edit_pos = 0;
	      if (_edit_pos > static_cast<int>(_SuperT::_text.size())) _edit_pos = static_cast<int>(_SuperT::_text.size());
	      _SuperT::invalidate();
        _SuperT::on_wm_keydown(key, k);

      };

      const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

    private:

      text_metrics _text_metrics;
      // _edit_pos tracks the index into _text for the edit cursor 
      int _edit_pos = 0;
      // _print_pos helps ensure the cursor is always visible e.g. text length > window length
      int _print_pos = 0;
    };
  }

  template <> struct policy_traits<policy::isa_textbox>{
    using requires = policy_list<policy::has_text, policy::wm_keydown, policy::wm_char, policy::has_cursor, policy::has_caret,
      policy::has_focus, policy::isa_label>;
  };

  struct textbox : window_impl<textbox, policy::isa_textbox>{
    explicit textbox(iwindow * pParent) : window_impl(pParent){}
  };

}
