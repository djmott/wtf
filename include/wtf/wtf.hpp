#pragma once

#define NOMINMAX 1

#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <algorithm>
#include <stdexcept>
#include <string>
#include <cassert>
#include <typeinfo>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <iterator>
#include <sstream>
#include <mutex>


namespace wtf {

	using tstring = std::basic_string<TCHAR>;
	using tstringstream = std::basic_stringstream<TCHAR>;

	enum class coord_frame {
		screen,
		client,
	};

	template <typename _ImplT> class window;
  
	using iwindow = window<void>;

	//this is the internal 'hidden' namespace not for external consumption
	namespace _ {
		static std::mutex& _active_forms_lock() {
			static std::mutex _forms_lock;
			return _forms_lock;
		}

		static std::vector<const iwindow*>& _active_forms() {
			static std::vector<const iwindow*> _forms;
			return _forms;
		}

		extern "C" HINSTANCE__ __ImageBase;
		inline static HINSTANCE instance_handle(){ return &__ImageBase; }
	}


	static const std::vector<const iwindow*>& active_forms() { return _::_active_forms(); }

	template <template <typename> typename ... _Policies> struct policy_list;
	template <template <typename> typename> struct policy_traits {
		using requires = policy_list<>;
	};
}


#include "exception.hpp"
#include "message_box.hpp"
#include "callback.hpp"
#include "point.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "size.hpp"
#include "icon.hpp"
#include "cursor.hpp"
#include "brush.hpp"
#include "pen.hpp"
#include "region.hpp"
#include "font.hpp"
#include "window_message.hpp"


#include "_/window_class_ex.hpp"
#include "_/device_context.hpp"
#include "_/msg_names.hpp"
#include "_/weak_enum.hpp"
#include "_/text_metrics.hpp"
#include "_/paint_struct.hpp"
#include "_/SystemParameters.hpp"
#include "_/message.hpp"


#include "window.hpp"
#include "policy_list.hpp"
#include "window_impl.hpp"


#include "messages/messages.hpp"
#include "messages/wm_activate.hpp"
#include "messages/wm_char.hpp"
#include "messages/wm_close.hpp"
#include "messages/wm_create.hpp"
#include "messages/wm_dblclick.hpp"
#include "messages/wm_destroy.hpp"
#include "messages/wm_erasebkgnd.hpp"
#include "messages/wm_geticon.hpp"
#include "messages/wm_keydown.hpp"
#include "messages/wm_keyup.hpp"
#include "messages/wm_killfocus.hpp"
#include "messages/wm_mouse_down.hpp"
#include "messages/wm_mouse_leave.hpp"
#include "messages/wm_mouse_move.hpp"
#include "messages/wm_mouse_up.hpp"
#include "messages/wm_mouse_wheel.hpp"
#include "messages/wm_move.hpp"
#include "messages/wm_nccalcsize.hpp"
#include "messages/wm_ncpaint.hpp"
#include "messages/wm_paint.hpp"
#include "messages/wm_setcursor.hpp"
#include "messages/wm_setfocus.hpp"
#include "messages/wm_showwindow.hpp"
#include "messages/wm_size.hpp"
#include "messages/wm_sizing.hpp"
#include "messages/wm_timer.hpp"



#include "policies/has_background.hpp"
#include "policies/has_border.hpp"
#include "policies/has_caret.hpp"
#include "policies/has_click.hpp"
#include "policies/has_close.hpp"
#include "policies/has_cursor.hpp"
#include "policies/has_focus.hpp"
#include "policies/has_font.hpp"
#include "policies/has_icon.hpp"
#include "policies/has_invalidate.hpp"
#include "policies/has_move.hpp"
#include "policies/has_orientation.hpp"
#include "policies/has_repeat_click.hpp"
#include "policies/has_show.hpp"
#include "policies/has_size.hpp"
#include "policies/has_text.hpp"
#include "policies/has_timer.hpp"
#include "policies/has_titlebar.hpp"
#include "policies/has_zorder.hpp"

#include "ui/panel.hpp"
#include "ui/label.hpp"
#include "ui/button.hpp"
#include "ui/scroll_bar.hpp"
#include "ui/checkbox.hpp"
#include "ui/form.hpp"
#include "ui/listbox.hpp"
#include "ui/progress_bar.hpp"
#include "ui/split_container.hpp"
#include "ui/tab_container.hpp"
#include "ui/textbox.hpp"
#include "ui/toggle_button.hpp"
#include "ui/tree.hpp"
