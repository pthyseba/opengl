project(render.scripting)

add_library(scripting STATIC ScriptHost.cc)
target_include_directories(scripting PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/.. /usr/include/squirrel3)
target_link_libraries(scripting PRIVATE gfx squirrel3 sqstdlib3)
