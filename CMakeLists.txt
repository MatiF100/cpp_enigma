cmake_minimum_required(VERSION 3.20)
project(CPPProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -lpthread -ldl -lm -lGL -lX")
add_executable(CPPProject main.cpp Drum.cpp Drum.h Plugboard.cpp Plugboard.h DrumAssembly.cpp DrumAssembly.h DrumAssemblyK.cpp DrumAssemblyK.h Machine.cpp Machine.h ImGUI/imgui.cpp ImGUI/imgui_draw.cpp ImGUI/imgui_internal.h ImGUI/imgui_tables.cpp ImGUI/imgui_widgets.cpp ImGUI/imgui_impl_opengl3.cpp ImGUI/imgui_impl_opengl3.h ImGUI/imgui_demo.cpp ImGUI/imgui_impl_glfw.cpp ImGUI/imgui_impl_glfw.h glad.c ImGUI/imgui_stdlib.cpp ImGUI/imgui_stdlib.h)

include_directories("${CMAKE_SOURCE_DIR}/include")
target_link_libraries(CPPProject ${CMAKE_SOURCE_DIR}/lib/glfw3.lib)
#include_directories("G:/wxWidgets-3.1.5/include")
#include_directories("G:/wxWidgets-3.1.5/cmake-build-debug/lib/wx/include/msw-unicode-3.1")
#target_link_libraries(CPPProject "G:/wxWidgets-3.1.5/cmake-build-debug/lib/wxmsw315ud_core_gcc_custom.dll")
#target_link_libraries(CPPProject "G:/wxWidgets-3.1.5/cmake-build-debug/lib/wxbase315ud_gcc_custom.dll")
