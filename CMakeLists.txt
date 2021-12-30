cmake_minimum_required(VERSION 3.20)
project(CPPProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(CPPProject main.cpp Drum.cpp Drum.h Plugboard.cpp Plugboard.h DrumAssembly.cpp DrumAssembly.h DrumAssemblyK.cpp DrumAssemblyK.h Machine.cpp Machine.h)

#include_directories("G:/wxWidgets-3.1.5/include")
#include_directories("G:/wxWidgets-3.1.5/cmake-build-debug/lib/wx/include/msw-unicode-3.1")
#target_link_libraries(CPPProject "G:/wxWidgets-3.1.5/cmake-build-debug/lib/wxmsw315ud_core_gcc_custom.dll")
#target_link_libraries(CPPProject "G:/wxWidgets-3.1.5/cmake-build-debug/lib/wxbase315ud_gcc_custom.dll")
