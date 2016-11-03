/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once

namespace wtf{

  namespace policy{

    template <typename _SuperT>
    struct isa_textbox : _SuperT{

      const tstring &text() const override{ return _text; }
      void text(const tstring & newval) override{
        _text = newval;
        _SuperT::invalidate();
      }

      bool accepts_tab() const{ return _accepts_tab; }
      void accepts_tab(bool newval){
        if (newval == _accepts_tab) return;
        _accepts_tab = newval;
        _SuperT::invalidate();
      }

    protected:
      
      explicit isa_textbox(window * pParent) : _SuperT(pParent){}

      void on_wm_create() override{
        _SuperT::background_brush(brush::system_brush(system_colors::window));
        _SuperT::border_style(border_styles::lowered);
        _SuperT::text_vertical_alignment(text_vertical_alignments::top);
        _SuperT::text_horizontal_alignment(text_horizontal_alignments::left);
        _SuperT::caret_width(1);
        _SuperT::auto_draw_text(false);
        _SuperT::caret_blink_rate(300);
        _SuperT::multiline(false);
        _SuperT::on_wm_create();
      };


      void on_wm_paint(const _::device_context& dc, const _::paint_struct& ps) override{
        auto otext_metrics = wtf::_::text_metrics::get(dc);
        _SuperT::draw_text(dc, ps.client(), _SuperT::_text.c_str() + _print);
        point<coord_frame::client> oCaretPos{ 0, otext_metrics.tmHeight };
        if (_edit > _print){
          auto oTextSize = dc.get_text_extent(_SuperT::_text.c_str() + _print, _edit - _print);
          oCaretPos.x = oTextSize.cx; 
        }
        _SuperT::on_wm_paint(dc, ps);
        _SuperT::create_caret();
        _SuperT::caret_height(otext_metrics.tmHeight);
        _SuperT::caret_visible(true);
        _SuperT::caret_position(oCaretPos);
      };

      void on_wm_size(const point<coord_frame::client>& p) override{
        _size = rect<coord_frame::client>::get(*this).dimensions();
        _SuperT::on_wm_size(p);
      }


      void on_wm_mouse_down(const mouse_msg_param& p) override{
        _SuperT::set_focus();
        _SuperT::on_wm_mouse_up(p);
      };


      void on_wm_char(UINT character, keyboard_msg_param k) override{
        std::cout << std::hex << "on_wm_char character: " << character << '\n';
        switch (character){
          case VK_ESCAPE:
          case VK_BACK:
          case VK_TAB:
          case _T('\r'):
          case _T('\n'):
            break;
          default:
          {
            _SuperT::_text.insert(_edit, 1, static_cast<TCHAR>(character));
            ++_edit;
            auto oDC = _::device_context::get_client(*this);
            for (;;){              
              auto oTextSize = oDC.get_text_extent(_SuperT::_text.c_str() + _print, _edit - _print);
              if (_print >= _edit || oTextSize.cx < _size.x) break;
              ++_print;
            }
            break;
          }
        }
        _SuperT::invalidate();
        _SuperT::on_wm_char(character, k);
      };

      void on_wm_keydown(UINT key, keyboard_msg_param param) override{
        std::cout << std::hex << "on_wm_keydown key: " << key << '\n';
        const auto cstr = _SuperT::_text.c_str();
        auto cstrlen = _SuperT::_text.size();
        if (!cstrlen) return _SuperT::on_wm_keydown(key, param);
        switch (key){
          case VK_LEFT:
          case VK_BACK:
          case VK_DELETE:
          {
            if (!_edit) break;
            if (VK_DELETE != key) --_edit;
            if (VK_BACK == key || VK_DELETE == key) _SuperT::_text.erase(_edit, 1);
            auto oDC = _::device_context::get_client(*this);
            for (; _print && _print > _edit - 4; --_print){
              auto oTextSize = oDC.get_text_extent(cstr + _print, _edit - _print);
              if (oTextSize.cx >= _size.x) break;
            }
            break;
          }
          case VK_RIGHT:
          {
            if (_edit >= cstrlen) break;
            ++_edit;
            auto oDC = _::device_context::get_client(*this);
            for (;;){
              auto oTextSize = oDC.get_text_extent(cstr + _print, _edit - _print);
              if (_print >= _edit || oTextSize.cx < _size.x) break;
              ++_print;
            }
            break;
          }
          case VK_HOME:
          {
            _print = _edit = 0;
            break;
          }
          case VK_END:
          {
            _edit = _print = static_cast<int>(cstrlen);
            if (!cstrlen) break;
            auto oDC = _::device_context::get_client(*this);
            for (;;){
              auto oTextSize = oDC.get_text_extent(cstr + _print, _edit - _print);
              if (!_print || oTextSize.cx > _size.x) break;
              ++_print;
            }
            break;
          }
        }
        _SuperT::invalidate();
        _SuperT::on_wm_keydown(key, param);
      }

      const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

    private:

      //tracks the edit cursor 
      int _edit = 0;
      //tracks where printing begins
      int _print = 0;
      //control dimensions
      point<coord_frame::client> _size{ 0,0 };
    };
  }

  namespace _{
    template <> struct policy_traits<policy::isa_textbox>{
      using requires = policy_list<policy::isa_label, policy::has_cursor, policy::has_caret,
        policy::has_focus, policy::wm_keydown, policy::wm_char>;
    };

  }

  struct textbox : window_impl<textbox, policy::isa_textbox>{
    explicit textbox(window * pParent) : window_impl(pParent){}
  };

}
