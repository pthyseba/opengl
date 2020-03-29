project(render.sfx)

add_library(sfx STATIC SoundManager.cc)
target_include_directories(sfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(sfx PRIVATE openal alut pthread X11 dl)
