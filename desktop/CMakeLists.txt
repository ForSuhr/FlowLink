cmake_minimum_required(VERSION 3.5)

project(FlowLink VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Gui Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Gui Core Network REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        FlowLink.h
        FlowLink.cpp
        FlowLink.ui
        Chat.h
        Chat.cpp
        Chat.ui
        UdpReceiver.h
        UdpReceiver.cpp
        UdpSender.h
        UdpSender.cpp
        utilities/globals.h
        utilities/globals.cpp
        utilities/TableModel.h
        utilities/TableModel.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FlowLink
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(FlowLink SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(FlowLink
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(FlowLink PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../ads")
target_link_libraries(FlowLink PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
                                      Qt${QT_VERSION_MAJOR}::Core
                                      Qt${QT_VERSION_MAJOR}::Gui
                                      Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(FlowLink PRIVATE qt5ads64)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FlowLink)
endif()
