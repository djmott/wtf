cmake_minimum_required(VERSION 3.0)

project(WTF LANGUAGES CXX VERSION 0.0.1.0 )

find_package(Doxygen)

if("Debug" STREQUAL "${CMAKE_BUILD_TYPE}"  OR "RelWithDebInfo" STREQUAL "${CMAKE_BUILD_TYPE}")
    add_definitions(-DDEBUG -D_DEBUG)
else()
    add_definitions(-DNDEBUG)
endif()

option(WTF_UNICODE "Build UNICODE" TRUE)
if(WTF_UNICODE)
    add_definitions(-DUNICODE -D_UNICODE)
else()
    add_definitions(-DMBCS -D_MBCS)
endif()

include_directories(include)

option(WTF_BUILD_EXAMPLES "Build examples" FALSE)

if(WTF_BUILD_EXAMPLES)
  add_executable(wtf WIN32
      examples/examples.cpp
      include/wtf/_/coinitialize.hpp
      include/wtf/_/effects.hpp
      include/wtf/_/init_common_controls.hpp
      include/wtf/_/meta.hpp
      include/wtf/_/msg_names.hpp
      include/wtf/_/SystemParameters.hpp
      include/wtf/_/text_metrics.hpp
      include/wtf/_/weak_enum.hpp
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
      include/wtf/menu.hpp
      include/wtf/message.hpp
      include/wtf/message_box.hpp
      include/wtf/monitor.hpp
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
      include/wtf/controls/calendar.hpp
      include/wtf/controls/combobox.hpp
      include/wtf/controls/date_time.hpp
      include/wtf/controls/edit.hpp
      include/wtf/controls/hotkey.hpp
      include/wtf/controls/image_list.hpp
      include/wtf/controls/label.hpp
      include/wtf/controls/listbox.hpp
      include/wtf/controls/listview.hpp
      include/wtf/controls/pager.hpp
      include/wtf/controls/progressbar.hpp
      include/wtf/controls/property_sheet.hpp
      include/wtf/controls/rebar.hpp
      include/wtf/controls/richedit.hpp
      include/wtf/controls/statusbar.hpp
      include/wtf/controls/syslink.hpp
      include/wtf/controls/tab.hpp
      include/wtf/controls/task_dialog.hpp
      include/wtf/controls/tooltip.hpp
      include/wtf/controls/trackbar.hpp
      include/wtf/controls/tree.hpp
      include/wtf/controls/up_down.hpp
      include/wtf/messages/messages.hpp
      include/wtf/messages/nm_killfocus.hpp
      include/wtf/messages/nm_setfocus.hpp
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
      include/wtf/messages/wm_getminmaxinfo.hpp
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
      include/wtf/messages/wm_notifyformat.hpp
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
      include/wtf/policies/has_hscroll.hpp
      include/wtf/policies/has_icon.hpp
      include/wtf/policies/has_image.hpp
      include/wtf/policies/has_invalidate.hpp
      include/wtf/policies/has_move.hpp
      include/wtf/policies/has_orientation.hpp
      include/wtf/policies/has_owner_drawn_border.hpp
      include/wtf/policies/has_owner_drawn_font.hpp
      include/wtf/policies/has_owner_drawn_text.hpp
      include/wtf/policies/has_repeat_click.hpp
      include/wtf/policies/has_show.hpp
      include/wtf/policies/has_size.hpp
      include/wtf/policies/has_text.hpp
      include/wtf/policies/has_timer.hpp
      include/wtf/policies/has_titlebar.hpp
      include/wtf/policies/has_vscroll.hpp
      include/wtf/policies/has_zorder.hpp
  )
endif()

if(DOXYGEN_FOUND)
  option(WTF_BUILD_DOCS "Build doxygen documentation" NO)
else()
  set(WTF_BUILD_DOCS NO)
endif()

if(WTF_BUILD_DOCS AND DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile)
  add_custom_target(
    docs ALL
    COMMAND "${DOXYGEN_EXECUTABLE}" Doxyfile
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
