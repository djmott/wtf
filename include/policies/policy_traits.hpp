#pragma once
// traits ensure dependent policies are inherited in concrete classes if they're missing from the declaration
namespace wtf {
  namespace policy {

    template<> struct traits<has_activate>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_border> {
      using requirements = type_list<>;
    };

    template<> struct traits<has_caret> {
      using requirements = type_list<>;
    };

    template<> struct traits<has_click> {
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_close>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_create>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_cursor>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_dblclick>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_focus> {
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_font> {
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_icon>{
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_keyboard>{
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_mouse_down>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_mouse_leave>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_mouse_move>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_mouse_up>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_mouse_wheel>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_orientation>{
      using requirements = type_list<>;
    };

    template<> struct traits<has_button_border>{
      using requirements = type_list<has_mouse_up, has_mouse_move, has_mouse_down, has_border>;
    };

    template<> struct traits<has_paint>{
      using requirements = type_list<>;
    };


    template<> struct traits<has_show> {
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_size> {
      using requirements = type_list<>;    
    };
    
    template<> struct traits<has_text> {
      using requirements = type_list<has_font>;
    };

    template<> struct traits<has_timer> {
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_repeat_click>{
      using requirements = type_list<has_click, has_timer, has_mouse_down, has_mouse_move, has_mouse_up>;
    };

    template<> struct traits<has_titlebar>{
      using requirements = type_list<>;
    };
    
    template<> struct traits<has_zorder>{
      using requirements = type_list<>;
    };
  }
}
