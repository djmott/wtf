#pragma once
namespace wtf {
  namespace policy {
    template<> struct traits<has_border> {
      using requirements = list<>;
    };

    template<> struct traits<has_caret> {
      using requirements = list<>;
    };

    template<> struct traits<has_click> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_close> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_cursor> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_focus> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_font> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_icon>{
      using requirements = list<>;
    };
    
    template<> struct traits<has_keyboard>{
      using requirements = list<>;
    };
    
    template<> struct traits<has_mouse>{
      using requirements = list<>;
    };

    template<> struct traits<has_button_border>{
      using requirements = list<has_mouse, has_border>;
    };

    template<> struct traits<has_paint>{
      using requirements = list<>;
    };


    template<> struct traits<has_show> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_size> {
      using requirements = list<>;    
    };
    
    template<> struct traits<has_text> {
      using requirements = list<has_font>;
    };

    template<> struct traits<has_timer> {
      using requirements = list<>;
    };
    
    template<> struct traits<has_repeat_click>{
      using requirements = list<has_click, has_timer, has_mouse>;
    };

    template<> struct traits<has_titlebar>{
      using requirements = list<>;
    };
    
    template<> struct traits<has_zorder>{
      using requirements = list<>;
    };
  }
}
