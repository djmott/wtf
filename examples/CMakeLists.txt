cmake_minimum_required(VERSION 2.6)

add_custom_target(examples)

add_executable(form_example form_example.cpp)
add_dependencies(form_example examples)

add_executable(button_example button_example.cpp)
add_dependencies(button_example examples)

add_executable(label_example label_example.cpp)
add_dependencies(label_example examples)

add_executable(listbox_example listbox_example.cpp)
add_dependencies(listbox_example examples)

add_executable(textbox_example textbox_example.cpp)
add_dependencies(textbox_example examples)

add_executable(tab_example tab_example.cpp)
add_dependencies(tab_example examples)

add_executable(eye_candy_example eye_candy_example.cpp)
add_dependencies(eye_candy_example examples)
