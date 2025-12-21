cmake_minimum_required(VERSION 4.2.0)
project(Center_of_Accident LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/Core/*.cpp
    ${PROJECT_SOURCE_DIR}/main/*.cpp
)

file(GLOB_RECURSE REFLECTION_SOURCES CONFIGURE_DEPENDS
    C:/VSProject/ReflectionLib/ReflectionLib/*.cpp
)

list(APPEND SOURCES ${REFLECTION_SOURCES})

add_executable(Center_of_Accident ${SOURCES})

# Створюємо окремий список файлів для Core, щоб CMake не плутав шляхи
file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/Core/*.cpp"
    "${PROJECT_SOURCE_DIR}/Core/*.h"
    "${PROJECT_SOURCE_DIR}/Core/*.hpp"
)

# Автоматичне дерево фільтрів (сама створить UI, Game, WorldObjects тощо)
source_group(TREE "${PROJECT_SOURCE_DIR}/Core" PREFIX "Core" FILES ${CORE_FILES})

# Ручні фільтри для того, що поза папкою Core
source_group("main" FILES "${PROJECT_SOURCE_DIR}/main/main.cpp")
source_group("ReflectionLib" FILES ${REFLECTION_SOURCES})

# Оновлюємо таргет (додаємо всі файли, щоб вони були в проекті)
target_sources(Center_of_Accident PRIVATE ${CORE_FILES} ${REFLECTION_SOURCES})

file(GLOB_RECURSE CORE_DIRS LIST_DIRECTORIES true "${PROJECT_SOURCE_DIR}/Core/*")
set(INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/Core
    ${PROJECT_SOURCE_DIR}/main/ThirdParty
    C:/VSProject/ReflectionLib/ReflectionLib
    C:/VSProject/SDK/SFML-3.0.0/include
    C:/VSProject/SDK/asio-1.36.0/include
    C:/VSProject/SimpleExampleNetwork/NetCommon
    ${CORE_DIRS}
)

target_include_directories(Center_of_Accident PRIVATE ${INCLUDE_DIRS})

target_compile_definitions(Center_of_Accident PRIVATE SFML_STATIC)

# Debug
target_link_libraries(Center_of_Accident PRIVATE
    $<$<CONFIG:Debug>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-s-d.lib>
    $<$<CONFIG:Debug>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-s-d.lib>
    $<$<CONFIG:Debug>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-s-d.lib>
    $<$<CONFIG:Debug>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-s-d.lib>
)

# Release
target_link_libraries(Center_of_Accident PRIVATE
    $<$<CONFIG:Release>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-s.lib>
    $<$<CONFIG:Release>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-s.lib>
    $<$<CONFIG:Release>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-s.lib>
    $<$<CONFIG:Release>:C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-s.lib>
)

target_link_libraries(Center_of_Accident PRIVATE
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-graphics-s-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-window-s-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-system-s-d.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/sfml-audio-s-d.lib

    # Зовнішні бібліотеки для статичного SFML
    C:/VSProject/SDK/SFML-3.0.0/lib/freetype.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/flac.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/vorbis.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/vorbisfile.lib
    C:/VSProject/SDK/SFML-3.0.0/lib/ogg.lib
    #C:/VSProject/SDK/SFML-3.0.0/lib/openal32.lib

    # Стандартні Windows бібліотеки
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

# Після білду гри Center_of_Accident копіюємо всю папку Resources
add_custom_command(TARGET Center_of_Accident POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/Resources"
        "$<TARGET_FILE_DIR:Center_of_Accident>/Resources"
    COMMENT "Copying all resources..."
)

#set(SFML_DLLS
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-graphics-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-window-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-system-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-audio-3.dll"
#)
#set(SFML_DLLS_DEBUG
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-graphics-d-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-window-d-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-system-d-3.dll"
#    "C:/VSProject/SDK/SFML-3.0.0/bin/sfml-audio-d-3.dll"
#)
#
#if(CMAKE_BUILD_TYPE STREQUAL "Debug")
#    foreach(dll ${SFML_DLLS_DEBUG})
#        add_custom_command(TARGET Center_of_Accident POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" $<TARGET_FILE_DIR:Center_of_Accident>
#        )
#    endforeach()
#else()
#    foreach(dll ${SFML_DLLS})
#        add_custom_command(TARGET Center_of_Accident POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" $<TARGET_FILE_DIR:Center_of_Accident>
#        )
#    endforeach()
#endif()