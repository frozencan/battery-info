cmake_minimum_required(VERSION 3.20)
project(battery-info VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Qml
    Quick
    QuickControls2
    DBus
)

if (COMMAND qt_policy)
    qt_policy(SET QTP0004 NEW)
endif()

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(battery-info
    src/main.cpp
    src/batterydevice.h
    src/batterydevice.cpp
    src/batterymanager.h
    src/batterymanager.cpp
)

set_source_files_properties(qml/Theme.qml PROPERTIES
    QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(battery-info
    URI BatteryApp
    VERSION 1.0
    QML_FILES
        qml/Theme.qml
        qml/Main.qml
        qml/EditorialCard.qml
        qml/NavigationHeader.qml
        qml/BatteryGauge.qml
        qml/OverviewPage.qml
        qml/HistoryPage.qml
        qml/HealthPage.qml
        qml/DiagnosticsPage.qml
)

target_link_libraries(battery-info PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::DBus
)

install(TARGETS battery-info
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
