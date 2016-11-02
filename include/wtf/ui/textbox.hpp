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
      
      explicit isa_textbox(iwindow * pParent) : _SuperT(pParent){}

      void on_wm_create() override{
        _SuperT::background_brush(brush::system_brush(system_colors::window));
        _SuperT::border_style(border_styles::lowered);
        _SuperT::text_vertical_alignment(text_vertical_alignments::top);
        _SuperT::text_horizontal_alignment(text_horizontal_alignments::left);
        _SuperT::caret_width(2);
        _SuperT::auto_draw_text(false);
        _SuperT::caret_blink_rate(250);
        _SuperT::on_wm_create();
      };


      void on_wm_paint(const _::device_context& dc, const _::paint_struct& ps) override{
        auto otext_metrics = wtf::_::text_metrics::get(dc);
        _SuperT::draw_text(dc, ps.client(), _SuperT::_text.c_str() + _print);
        auto oCaretPos = dc.get_text_extent(_SuperT::_text.c_str() + _print, _print - _edit);
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
        switch (character){
          case _T('\9'):{
            backspace(); break;
          }
          case _T('\n'):{

          }
          default:{
            assert(std::isprint(character, std::locale("en_US")));
          }
        }
        _SuperT::invalidate();
        _SuperT::on_wm_char(character, k);
      };

      void on_wm_keydown(UINT key, keyboard_msg_param k) override{
        switch (key){
          case VK_LEFT:{

          }
          case VK_RIGHT:{

          }
          case VK_UP:{

          }
          case VK_DOWN:{

          }
          case VK_BACK:{

          }
          case VK_HOME:{

          }
          case VK_END:{

          }
        }
        _SuperT::invalidate();
        _SuperT::on_wm_keydown(key, k);

      };

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

  template <> struct policy_traits<policy::isa_textbox>{
    using requires = policy_list<policy::isa_label, policy::has_cursor, policy::has_caret,
      policy::has_focus, policy::wm_keydown, policy::wm_char>;
  };

  struct textbox : window_impl<textbox, policy::isa_textbox>{
    explicit textbox(iwindow * pParent) : window_impl(pParent){}
  };


}
