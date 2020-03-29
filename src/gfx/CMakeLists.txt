project(render.gfx)

add_library(gfx STATIC Mesh.cc Shader.cc WindowManager.cc Camera.cc DefaultModel.cc)
target_include_directories(gfx PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
target_link_libraries(gfx PRIVATE glfw GL GLU openal alut pthread X11 dl)
