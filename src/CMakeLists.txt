project(test)

add_executable(test test.cc glad.c)
target_include_directories(test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(test PRIVATE glfw3 GL GLU pthread X11 dl)
