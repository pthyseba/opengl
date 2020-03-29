project(render)

add_subdirectory(gfx)
add_subdirectory(sfx)

add_executable(render glad.c Controller.cc render.cc)
target_include_directories(render PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(render PRIVATE sfx gfx glfw GL GLU openal alut pthread X11 dl)
