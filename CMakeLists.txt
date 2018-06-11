cmake_minimum_required(VERSION 2.6)

project(wtf)

find_package(Doxygen)

if("Debug" STREQUAL "${CMAKE_BUILD_TYPE}"  OR "RelWithDebInfo" STREQUAL "${CMAKE_BUILD_TYPE}")
	add_definitions(-DDEBUG -D_DEBUG)
else()
	add_definitions(-DNDEBUG)
endif()


include_directories(include)

add_executable(wtf WIN32
	examples/tabs.cpp
	include/wtf/brush.hpp
	include/wtf/callback.hpp
	include/wtf/color.hpp
	include/wtf/cursor.hpp
	include/wtf/device_context.hpp
	include/wtf/exception.hpp
	include/wtf/font.hpp
	include/wtf/font_family.hpp
	include/wtf/form.hpp
	include/wtf/icon.hpp
	include/wtf/message.hpp
	include/wtf/message_box.hpp
	include/wtf/paint_struct.hpp
	include/wtf/pen.hpp
	include/wtf/point.hpp
	include/wtf/rect.hpp
	include/wtf/region.hpp
	include/wtf/size.hpp
	include/wtf/window.hpp
	include/wtf/window_class.hpp
	include/wtf/window_message.hpp
	include/wtf/wtf.hpp
	include/wtf/controls/button.hpp
	include/wtf/controls/combobox.hpp
	include/wtf/controls/edit.hpp
	include/wtf/controls/label.hpp
	include/wtf/controls/policies/
	include/wtf/controls/tab.hpp
	include/wtf/controls/tree.hpp
	include/wtf/controls/policies/has_font.hpp
	include/wtf/controls/policies/has_text.hpp
	include/wtf/custom/button.hpp
	include/wtf/custom/checkbox.hpp
	include/wtf/custom/combobox.hpp
	include/wtf/custom/inputbox.hpp
	include/wtf/custom/label.hpp
	include/wtf/custom/list.hpp
	include/wtf/custom/listbox.hpp
	include/wtf/custom/menu.hpp
	include/wtf/custom/panel.hpp
	include/wtf/custom/progress_bar.hpp
	include/wtf/custom/scroll_bar.hpp
	include/wtf/custom/scroll_panel.hpp
	include/wtf/custom/split_container.hpp
	include/wtf/custom/tab_container.hpp
	include/wtf/custom/textbox.hpp
	include/wtf/custom/toggle_button.hpp
	include/wtf/custom/tree.hpp
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
)

if(DOXYGEN_FOUND)
  option(BUILD_DOCS "Build doxygen documentation" NO)
else()
  set(BUILD_DOCS NO)
endif()

if(BUILD_DOCS AND DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
  add_custom_target(
    docs COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile
    COMMENT "Generating Doxygen API Documentation" VERBATIM
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs"
	SOURCES
		LICENSE.md
		README.md
		docs/using_wtf.md
		docs/hierarchy_gen.md
		docs/message_handling.md
		docs/Doxyfile.in
		docs/footer.html
		docs/Doxyfile
  )
endif()
