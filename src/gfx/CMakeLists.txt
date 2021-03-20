project(render.gfx)

add_library(gfx STATIC Mesh.cc Shader.cc WindowManager.cc Camera.cc Model.cc DefaultModel.cc JSONModel.cc FontManager.cc)
target_include_directories(gfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include /usr/include/freetype2 ${CMAKE_SOURCE_DIR} /usr/include/squirrel3)
target_link_libraries(gfx PRIVATE glfw GL GLU openal alut pthread freetype X11 dl polyclipping)
