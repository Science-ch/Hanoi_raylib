cmake_minimum_required(VERSION 3.10)
project(Hanoi C CXX)

set(CMAKE_CXX_STANDARD 11)

set(RAYLIB_INCLUDE_DIR D:/Libraries/Windows/raylib-5.5_win64_mingw-w64/include)
set(RAYGUI_INCLUDE_DIR D:/Libraries/Windows/raygui/src)
set(RAYLIB_LIBRARY D:/Libraries/Windows/raylib-5.5_win64_mingw-w64/lib/libraylib.a)

file(GLOB CPP_SOURCES "src/*.cpp")

add_executable(Hanoi ${C_SOURCES} ${CPP_SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/include)
target_include_directories(Hanoi PRIVATE ${RAYLIB_INCLUDE_DIR} ${RAYGUI_INCLUDE_DIR})

target_link_libraries(Hanoi PRIVATE
  ${RAYLIB_LIBRARY}
  winmm gdi32 opengl32 ws2_32
)

set_target_properties(Hanoi PROPERTIES
  WIN32_EXECUTABLE FALSE
)

if(WIN32)
  set_target_properties(Hanoi PROPERTIES
    WIN32_EXECUTABLE FALSE
    LINK_FLAGS "-static"
  )
endif()