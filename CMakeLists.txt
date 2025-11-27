cmake_minimum_required(VERSION 3.10)
project(Hanoi C CXX)

set(CMAKE_CXX_STANDARD 11)

if (WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_SOURCE_DIR}/resource.rc")
endif()

set(RAYLIB_INCLUDE_DIR D:/Libraries/Windows/raylib-5.5_win64_mingw-w64/include)
set(RAYGUI_INCLUDE_DIR D:/Libraries/Windows/raygui/src)
set(RAYLIB_LIBRARY D:/Libraries/Windows/raylib-5.5_win64_mingw-w64/lib/libraylib.a)

file(GLOB C_SOURCES "src/*.c")
file(GLOB CPP_SOURCES "src/*.cpp")

add_executable(Hanoi ${C_SOURCES} ${CPP_SOURCES} ${APP_ICON_RESOURCE_WINDOWS})

include_directories(${PROJECT_SOURCE_DIR}/include)
target_include_directories(Hanoi PRIVATE ${RAYLIB_INCLUDE_DIR} ${RAYGUI_INCLUDE_DIR})

target_link_libraries(Hanoi PRIVATE
  ${RAYLIB_LIBRARY}
  winmm gdi32 opengl32 ws2_32
)

if(WIN32)
  set_target_properties(Hanoi PROPERTIES
    WIN32_EXECUTABLE YES
    LINK_FLAGS "-static"
  )
endif()