/** @file
@copyright David Mott (c) 2016. Distributed under the Boost Software License Version 1.0. See LICENSE.md or http://boost.org/LICENSE_1_0.txt for details.
*/
#pragma once
#if 0

namespace wtf{
  namespace custom{
  namespace policy{

    template <typename _super_t>
    struct isa_textbox : _super_t{


    protected:
      
      explicit isa_textbox(window * pParent)  : _super_t(pParent){}

      void on_wm_create() override{
        _super_t::background_brush(brush::system_brush(system_colors::window));
        _super_t::border_style(border_styles::lowered);
        _super_t::text_vertical_alignment(text_vertical_alignments::top);
        _super_t::text_horizontal_alignment(text_horizontal_alignments::left);
        _super_t::caret_width(1);
        _super_t::auto_draw_text(false);
        _super_t::caret_blink_rate(300);
        _super_t::multiline(false);
        _super_t::on_wm_create();
      };


      void on_wm_paint(const device_context& dc, const paint_struct& ps) override{
        auto otext_metrics = wtf::_::text_metrics::get(dc);
        _super_t::draw_text(dc, ps.client(), _super_t::text().c_str() + _print);
        point<coord_frame::client> oCaretPos{ 0, otext_metrics.tmHeight };
        if (_edit > _print){
          auto oTextSize = dc.get_text_extent(_super_t::text().c_str() + _print, _edit - _print);
          oCaretPos.x = oTextSize.cx; 
        }
        _super_t::on_wm_paint(dc, ps);
        _super_t::create_caret();
        _super_t::caret_height(otext_metrics.tmHeight);
        _super_t::caret_visible(true);
        _super_t::caret_position(oCaretPos);
      };

      void on_wm_size(const point<coord_frame::client>& p) override{
        _size = rect<coord_frame::client>::get(*this).dimensions();
        _super_t::on_wm_size(p);
      }


      void on_wm_mouse_down(const mouse_msg_param<coord_frame::client>& p) override{
        _super_t::set_focus();
        _super_t::on_wm_mouse_up(p);
      };


      void on_wm_char(UINT character, keyboard_msg_param k) override{
        switch (character){
          case VK_ESCAPE:
          case VK_BACK:
          case VK_TAB:
          case _T('\r'):
          case _T('\n'):
            break;
          default:
          {
            _super_t::text().insert(_edit, 1, static_cast<TCHAR>(character));
            ++_edit;
            auto oDC = device_context::get_client(*this);
            for (;;){              
              auto oTextSize = oDC.get_text_extent(_super_t::text().c_str() + _print, _edit - _print);
              if (_print >= _edit || oTextSize.cx < _size.x) break;
              ++_print;
            }
            break;
          }
        }
        _super_t::invalidate();
        _super_t::on_wm_char(character, k);
      };

      void on_wm_keydown(UINT key, keyboard_msg_param param) override{
        const auto cstr = _super_t::text().c_str();
        auto cstrlen = _super_t::text().size();
        if (!cstrlen) return _super_t::on_wm_keydown(key, param);
        switch (key){
          case VK_LEFT:
          case VK_BACK:
          case VK_DELETE:
          {
            if (!_edit) break;
            if (VK_DELETE != key) --_edit;
            if (VK_BACK == key || VK_DELETE == key) _super_t::text().erase(_edit, 1);
            auto oDC = device_context::get_client(*this);
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
            auto oDC = device_context::get_client(*this);
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
            auto oDC = device_context::get_client(*this);
            for (;;){
              auto oTextSize = oDC.get_text_extent(cstr + _print, _edit - _print);
              if (!_print || oTextSize.cx > _size.x) break;
              ++_print;
            }
            break;
          }
        }
        _super_t::invalidate();
        _super_t::on_wm_keydown(key, param);
      }
      
      const wtf::cursor& cursor_pointer() const override{ return cursor::global(cursor::style::ibeam); }

    private:

      //tracks the edit cursor 
      size_t _edit = 0;
      //tracks where printing begins
      size_t _print = 0;
      //control dimensions
      point<coord_frame::client> _size{ 0,0 };
    };
  }



  struct textbox : window_impl<textbox,
    policy::isa_textbox,
    policy::isa_label, 
    policy::has_cursor, 
    policy::has_caret,
    policy::has_focus,
    policy::has_text,
    policy::has_font,
    policy::wm_keydown, 
    policy::wm_char,
    policy::wm_killfocus,
    policy::wm_setfocus,
    policy::wm_setcursor,
    policy::wm_create,
    policy::wm_size,
    policy::wm_mouse_down
  >{
    explicit textbox(window * pParent)  : window_impl(pParent){}
  };

}
}
#endif