cmake_minimum_required(VERSION 2.6)

project(wtf)

find_package(Doxygen)

option(WTF_DEBUG_MESSAGES "Debug windows messages" FALSE)
mark_as_advanced(WTF_DEBUG_MESSAGES)
if(WTF_DEBUG_MESSAGES)
	add_definitions(-D__WTF_DEBUG_MESSAGES__=1)
endif()

include_directories(include)

add_executable(wtf WIN32
	examples/tabs.cpp
	include/wtf/brush.hpp
	include/wtf/callback.hpp
	include/wtf/color.hpp
	include/wtf/combobox.hpp
	include/wtf/controls/button.hpp
	include/wtf/controls/combobox.hpp
	include/wtf/controls/edit.hpp
	include/wtf/controls/tab.hpp
	include/wtf/controls/policies/has_font.hpp
	include/wtf/controls/policies/has_text.hpp
	include/wtf/cursor.hpp
	include/wtf/device_context.hpp
	include/wtf/exception.hpp
	include/wtf/font.hpp
	include/wtf/font_family.hpp
	include/wtf/icon.hpp
	include/wtf/message.hpp
	include/wtf/messages/messages.hpp
	include/wtf/messages/wm_activate.hpp
	include/wtf/messages/wm_char.hpp
	include/wtf/messages/wm_close.hpp
	include/wtf/messages/wm_command.hpp
	include/wtf/messages/wm_create.hpp
	include/wtf/messages/wm_dblclick.hpp
	include/wtf/messages/wm_destroy.hpp
	include/wtf/messages/wm_enable.hpp
	include/wtf/messages/wm_erasebkgnd.hpp
	include/wtf/messages/wm_geticon.hpp
	include/wtf/messages/wm_keydown.hpp
	include/wtf/messages/wm_keyup.hpp
	include/wtf/messages/wm_killfocus.hpp
	include/wtf/messages/wm_mouse_down.hpp
	include/wtf/messages/wm_mouse_leave.hpp
	include/wtf/messages/wm_mouse_move.hpp
	include/wtf/messages/wm_mouse_up.hpp
	include/wtf/messages/wm_mouse_wheel.hpp
	include/wtf/messages/wm_move.hpp
	include/wtf/messages/wm_moving.hpp
	include/wtf/messages/wm_nccalcsize.hpp
	include/wtf/messages/wm_ncmouse_down.hpp
	include/wtf/messages/wm_ncmouse_leave.hpp
	include/wtf/messages/wm_ncmouse_move.hpp
	include/wtf/messages/wm_ncmouse_up.hpp
	include/wtf/messages/wm_ncpaint.hpp
	include/wtf/messages/wm_notify.hpp
	include/wtf/messages/wm_paint.hpp
	include/wtf/messages/wm_setcursor.hpp
	include/wtf/messages/wm_setfocus.hpp
	include/wtf/messages/wm_showwindow.hpp
	include/wtf/messages/wm_size.hpp
	include/wtf/messages/wm_sizing.hpp
	include/wtf/messages/wm_timer.hpp
	include/wtf/message_box.hpp
	include/wtf/paint_struct.hpp
	include/wtf/pen.hpp
	include/wtf/point.hpp
	include/wtf/policies/has_background.hpp
	include/wtf/policies/has_border.hpp
	include/wtf/policies/has_caret.hpp
	include/wtf/policies/has_click.hpp
	include/wtf/policies/has_close.hpp
	include/wtf/policies/has_cursor.hpp
	include/wtf/policies/has_enable.hpp
	include/wtf/policies/has_focus.hpp
	include/wtf/policies/has_font.hpp
	include/wtf/policies/has_icon.hpp
	include/wtf/policies/has_image.hpp
	include/wtf/policies/has_invalidate.hpp
	include/wtf/policies/has_move.hpp
	include/wtf/policies/has_orientation.hpp
	include/wtf/policies/has_repeat_click.hpp
	include/wtf/policies/has_show.hpp
	include/wtf/policies/has_size.hpp
	include/wtf/policies/has_text.hpp
	include/wtf/policies/has_timer.hpp
	include/wtf/policies/has_titlebar.hpp
	include/wtf/policies/has_zorder.hpp
	include/wtf/rect.hpp
	include/wtf/region.hpp
	include/wtf/size.hpp
	include/wtf/ui/button.hpp
	include/wtf/ui/checkbox.hpp
	include/wtf/ui/form.hpp
	include/wtf/ui/inputbox.hpp
	include/wtf/ui/label.hpp
	include/wtf/ui/list.hpp
	include/wtf/ui/listbox.hpp
	include/wtf/ui/menu.hpp
	include/wtf/ui/panel.hpp
	include/wtf/ui/progress_bar.hpp
	include/wtf/ui/scroll_bar.hpp
	include/wtf/ui/scroll_panel.hpp
	include/wtf/ui/split_container.hpp
	include/wtf/ui/tab_container.hpp
	include/wtf/ui/textbox.hpp
	include/wtf/ui/toggle_button.hpp
	include/wtf/ui/tree.hpp
	include/wtf/window.hpp
	include/wtf/window_class_ex.hpp
	include/wtf/window_message.hpp
	include/wtf/wtf.hpp
	include/wtf/_/effects.hpp
	include/wtf/_/init_common_controls.hpp
	include/wtf/_/msg_names.hpp
	include/wtf/_/SystemParameters.hpp
	include/wtf/_/text_metrics.hpp
	include/wtf/_/weak_enum.hpp
)




#add_subdirectory(examples)


if(DOXYGEN_FOUND)
  option(BUILD_DOCS "Build doxygen documentation" NO)
else()
  set(BUILD_DOCS NO)
endif()

if(BUILD_DOCS AND DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
  add_custom_target(
    docs COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
    COMMENT "Generating Doxygen API Documentation" VERBATIM
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
  )
endif()
