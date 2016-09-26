#pragma once

namespace wtf {

	template <typename _ImplT, WNDPROC _WndProc, UINT _ClassStyle, typename _IconT=icons::null_icon, typename _CursorT=cursors::null_cursor, typename _BrushT = brushes::null_brush, typename _MenuT = menus::null_menu, typename _SmallIconT = icons::null_icon>
		struct window_class_ex : WNDCLASSEX {
			static const UINT class_style = _ClassStyle;
			using icon_type = _IconT;
			using cursor_type = _CursorT;
			using brush_type = _BrushT;
			using menu_type = _MenuT;
			using small_icon_type = _SmallIconT;

			static window_class_ex& get() {
				static window_class_ex _window_class_ex;
				return _window_class_ex;
			}

			LPCTSTR name() {  return _class_name.c_str(); }

			~window_class_ex(){ UnregisterClass(_class_name.c_str(), reinterpret_cast<HINSTANCE>(&__ImageBase)); }

		protected:

			window_class_ex() {
  			//this goofy looking bit of code creates a unique class name in unicode or multibyte
  			std::string sTemp = "wtf" + std::to_string(typeid(_ImplT).hash_code());
  			for (auto ch : sTemp) {
    			_class_name.push_back(ch);
  			}
				cbSize = sizeof(WNDCLASSEX);
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
				exception::throw_lasterr_if(RegisterClassEx(this), [](ATOM x){ return 0 == x; });
			}

			icon_type _icon;
			cursor_type _cursor;
			brush_type _brush;
			menu_type _menu;
			small_icon_type _small_icon;
			std::basic_string<TCHAR> _class_name;
		};
}