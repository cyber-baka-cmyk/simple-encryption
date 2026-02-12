cmake_minimum_required(VERSION 3.16)
project(SimpleShuffleCipher C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 QUIET COMPONENTS Core Gui Widgets)
if (NOT Qt6_FOUND)
    find_package(Qt5 QUIET COMPONENTS Core Gui Widgets)
endif()
if(Qt6_FOUND OR Qt5_FOUND)
    message(STATUS "Qt found - GUI will be built")
else()
    message(WARNING "Qt not found. GUI will NOT be built. Only CLI and tests.")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(cipher_lib STATIC
    src/cipher.c
)
target_include_directories(cipher_lib PUBLIC include)

add_executable(cipher_app src/main.c)
target_link_libraries(cipher_app cipher_lib)

if(Qt6_FOUND OR Qt5_FOUND)
    add_executable(cipher_gui
        src/main.cpp
        src/MainWindow.cpp
        include/MainWindow.h
    )
    
    target_include_directories(cipher_gui PRIVATE include)
    
    if(Qt6_FOUND)
        target_link_libraries(cipher_gui
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
            cipher_lib
        )
        message(STATUS "Qt6 found - building GUI with Qt6")
    else()
        target_link_libraries(cipher_gui
            Qt5::Core
            Qt5::Gui
            Qt5::Widgets
            cipher_lib
        )
        message(STATUS "Qt5 found - building GUI with Qt5")
    endif()
    
    message(STATUS "GUI application will be built: cipher_gui")
else()
    message(WARNING "Qt not found. GUI application will not be built.")
endif()

install(TARGETS cipher_app RUNTIME DESTINATION bin)
if(Qt6_FOUND OR Qt5_FOUND)
    install(TARGETS cipher_gui RUNTIME DESTINATION bin)
endif()