#pragma once

namespace wtf{

  template <WNDPROC _WndProc, DWORD _ExWindowStyle, DWORD _WindowStyle, UINT _ClassStyle, typename _IconT, typename _CursorT, typename _BrushT, typename _MenuT, typename _SmallIconT>
  struct window_class_ex : WNDCLASSEXA{
    static const DWORD ex_window_style = _ExWindowStyle;
    static const DWORD window_style = _WindowStyle;
    static const UINT class_style = _ClassStyle;
    using icon_type = _IconT;
    using cursor_type = _CursorT;
    using brush_type = _BrushT;
    using menu_type = _MenuT;
    using small_icon_type = _SmallIconT;

    static window_class_ex& get(){
      static window_class_ex _window_class_ex;
      return _window_class_ex;
    }

    const char * name() const{ return _class_name.c_str(); }


  protected:

    window_class_ex() {
      _class_name = "wtf";
      _class_name += std::to_string(typeid(window_class_ex).hash_code());
      cbSize = sizeof(WNDCLASSEXA);
      style = class_style;
      lpfnWndProc = _WndProc;
      cbClsExtra = sizeof(window_class_ex*);
      cbWndExtra = sizeof(void*);
      hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
      hIcon = _icon;
      hCursor = _cursor;
      hbrBackground = _brush;
      lpszMenuName = nullptr;
      lpszClassName = name();
      hIconSm = _small_icon;
      exception::throw_lasterr_if(RegisterClassExA(this), [](ATOM x){ return 0 == x; });
    }

    icon_type _icon;
    cursor_type _cursor;
    brush_type _brush;
    menu_type _menu;
    small_icon_type _small_icon;
    std::string _class_name;
  private:

  };
}