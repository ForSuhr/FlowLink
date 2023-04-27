set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Gui Core Network REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Gui Core Network REQUIRED)

set(PROJECT_SOURCES
        Main.cpp
        FlowLink.h
        FlowLink.cpp
        FlowLink.ui
        ChatWindow.h
        ChatWindow.cpp
        ChatWindow.ui
        UdpSender.h
        UdpSender.cpp
        UdpReceiver.h
        UdpReceiver.cpp
        TcpSender.h
        TcpSender.cpp
        TcpReceiver.h
        TcpReceiver.cpp
        Globals.h
        Globals.cpp
        utils/TableModel.h
        utils/TableModel.cpp
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

target_include_directories(FlowLink PRIVATE "${CMAKE_SOURCE_DIR}/3rdparty/ads")
target_link_libraries(FlowLink PUBLIC Qt${QT_VERSION_MAJOR}::Widgets
                                      Qt${QT_VERSION_MAJOR}::Core
                                      Qt${QT_VERSION_MAJOR}::Gui
                                      Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(FlowLink PRIVATE qt5ads64)
target_link_libraries(FlowLink PRIVATE plog)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FlowLink)
endif()
