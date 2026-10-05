cmake_minimum_required(VERSION 4.2.0)

project(Center_of_Accident LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Допомагає IntelliSense та іншим інструментам бачити
# реальні параметри компіляції CMake.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# ============================================================
# Core sources
# ============================================================

file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/Core/*.cpp"
    "${PROJECT_SOURCE_DIR}/Core/*.h"
    "${PROJECT_SOURCE_DIR}/Core/*.hpp"
)


# ============================================================
# ReflectionLib sources
# ============================================================

file(GLOB_RECURSE REFLECTION_SOURCES CONFIGURE_DEPENDS
    "C:/VSProject/ReflectionLib/ReflectionLib/*.cpp"
)


# ============================================================
# Executable
# ============================================================

add_executable(Center_of_Accident
    "${PROJECT_SOURCE_DIR}/main/main.cpp"
    ${CORE_FILES}
    ${REFLECTION_SOURCES}
)


# ============================================================
# Visual Studio folder structure
# ============================================================

# Відтворюємо реальне дерево Core у Solution Explorer:
#
# Core
# ├── UI
# │   ├── Forms
# │   └── Widgets
# ├── Game
# │   └── World
# └── Network
#
# У дерево потрапляють тільки .h/.hpp/.cpp файли.
source_group(
    TREE "${PROJECT_SOURCE_DIR}/Core"
    PREFIX "Core"
    FILES ${CORE_FILES}
)

source_group(
    "main"
    FILES
        "${PROJECT_SOURCE_DIR}/main/main.cpp"
)

source_group(
    "ReflectionLib"
    FILES ${REFLECTION_SOURCES}
)


# ============================================================
# Include directories
# ============================================================

file(GLOB_RECURSE CORE_DIRS LIST_DIRECTORIES true
    CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/Core/*"
)

set(INCLUDE_DIRS
    "${PROJECT_SOURCE_DIR}/Core"
    "${PROJECT_SOURCE_DIR}/main/ThirdParty"
    "C:/VSProject/ReflectionLib/ReflectionLib"
    "C:/VSProject/SDK/SFML-3.0.0/include"
    "C:/VSProject/SDK/asio-1.36.0/include"
    "C:/VSProject/SimpleExampleNetwork/NetCommon"
    ${CORE_DIRS}
)

target_include_directories(Center_of_Accident PRIVATE
    ${INCLUDE_DIRS}
)


# ============================================================
# Compile definitions
# ============================================================

target_compile_definitions(Center_of_Accident PRIVATE
    SFML_STATIC
)


# ============================================================
# SFML - Debug
# ============================================================

target_link_libraries(Center_of_Accident PRIVATE

    $<$<CONFIG:Debug>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-s-d.lib
    >

    $<$<CONFIG:Debug>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-s-d.lib
    >

    $<$<CONFIG:Debug>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-s-d.lib
    >

    $<$<CONFIG:Debug>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-s-d.lib
    >
)


# ============================================================
# SFML - Release
# ============================================================

target_link_libraries(Center_of_Accident PRIVATE

    $<$<CONFIG:Release>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-s.lib
    >

    $<$<CONFIG:Release>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-s.lib
    >

    $<$<CONFIG:Release>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-s.lib
    >

    $<$<CONFIG:Release>:
        C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-s.lib
    >
)


# ============================================================
# SFML dependencies
# ============================================================

target_link_libraries(Center_of_Accident PRIVATE

    C:/VSProject/SDK/SFML-3.0.0/lib/freetype.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/flac.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/vorbis.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/vorbisfile.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/ogg.lib

    # Windows libraries
    opengl32.lib
    winmm.lib
    gdi32.lib
    user32.lib
    shell32.lib
    comdlg32.lib
    advapi32.lib
    ole32.lib
    oleaut32.lib
    uuid.lib
)


# ============================================================
# Copy Resources
# ============================================================

add_custom_command(TARGET Center_of_Accident POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Resources"
        "$<TARGET_FILE_DIR:Center_of_Accident>/Resources"

    COMMENT "Copying all resources..."
)
