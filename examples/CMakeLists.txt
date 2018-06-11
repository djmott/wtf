cmake_minimum_required(VERSION 2.6)


add_executable(buttons WIN32 buttons.cpp)
add_executable(combobox WIN32 combobox.cpp)
add_executable(edit WIN32
	edit.cpp
	${WTF_SOURCE_DIR}/include/wtf/controls/edit.hpp
	${WTF_SOURCE_DIR}/include/wtf/controls/policies/has_font.hpp
	${WTF_SOURCE_DIR}/include/wtf/controls/policies/has_text.hpp
	${WTF_SOURCE_DIR}/include/wtf/policies/has_move.hpp
)

#[[
add_example(edit)
add_example(events)
add_example(examples)
add_example(form_styles)
add_example(inputbox)
add_example(panels)
add_example(progressbar)
add_example(scrollbars)
add_example(simple_forms)
add_example(subclassing)
add_example(textbox)
add_example(tree)
]]