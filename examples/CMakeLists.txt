cmake_minimum_required(VERSION 2.6)

add_custom_target(examples)

add_executable(form_example form_example.cpp)
add_dependencies(form_example examples)

add_executable(button_example button_example.cpp)
add_dependencies(button_example examples)
