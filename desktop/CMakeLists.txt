set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Gui Core Network LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Gui Core Network LinguistTools REQUIRED)

set(PROJECT_SOURCES
        Main.cpp
        FlowLink.h
        FlowLink.cpp
        FlowLink.ui
        Globals.h
        Globals.cpp
        windows/WindowsGlobals.h
        windows/WindowsGlobals.cpp
        windows/ChatWindow.h
        windows/ChatWindow.cpp
        windows/ChatWindow.ui
        windows/PrefWindow.h
        windows/PrefWindow.cpp
        windows/ProgressWindow.h
        windows/ProgressWindow.cpp
        network/NetworkGlobals.h
        network/NetworkGlobals.cpp        
        network/NetworkManager.h
        network/NetworkManager.cpp
        network/UdpSender.h
        network/UdpSender.cpp
        network/UdpReceiver.h
        network/UdpReceiver.cpp
        network/TcpSender.h
        network/TcpSender.cpp
        network/TcpReceiver.h
        network/TcpReceiver.cpp
        network/WorkerThread.h
        network/WorkerThread.cpp
        utils/StyleSheet.h
        utils/StyleSheet.cpp 
        utils/QSettingsJSON.h
        utils/QSettingsJSON.cpp
        utils/ConfigParser.h
        utils/ConfigParser.cpp
        utils/TableModel.h
        utils/TableModel.cpp
        utils/TabBar.h
        utils/TabBar.cpp
        FlowLink.qrc
)

set(PROJECT_TRANSLATE
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/zh_CN.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/en_US.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/de_DE.ts
    )

set(TS_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/zh_CN.ts  
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/en_US.ts
    ${CMAKE_CURRENT_SOURCE_DIR}/translate/de_DE.ts
    )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FlowLink
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ${PROJECT_TRANSLATE}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(FlowLink ${PROJECT_SOURCES} ${PROJECT_TRANSLATE})
    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
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
