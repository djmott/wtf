#pragma once
namespace wtf {
  namespace policy {
    template<>
    struct traits<has_border> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_caret> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_click> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_close> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_cursor> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_focus> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_font> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_icon> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_mouse> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_paint> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_show> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_size> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_text> {
      using requirements = list<has_font>;
    };
    template<>
    struct traits<has_timer> {
      using requirements = list<>;
    };
    template<>
    struct traits<has_titlebar> {
      using requirements = list<>;
    };
  }
}
