project(render)

add_subdirectory(gfx)
add_subdirectory(sfx)
add_subdirectory(clipper)
add_subdirectory(scripting)

add_executable(render glad.c Controller.cc render.cc)
target_include_directories(render PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR})
target_link_libraries(render PRIVATE sfx gfx scripting glfw GL GLU openal alut pthread X11 dl)
