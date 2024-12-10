cmake_minimum_required(VERSION 3.16)
project(HomeQuest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Gui)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h mainwindow.ui
    loginwindow.cpp loginwindow.h
    listingdetail.cpp listingdetail.h
    profileview.cpp profileview.h
    user.cpp user.h
    registrationwindow.cpp registrationwindow.h
    housing.cpp housing.h
    database.cpp database.h
    filters.h
    distancemap.h
    passwordvalidator.h
    csvreader.h
    listings.csv
    resources.qrc
    Info.plist
)

qt_add_executable(HomeQuest MANUAL_FINALIZATION ${PROJECT_SOURCES})

target_link_libraries(HomeQuest PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(HomeQuest PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
    MACOSX_BUNDLE_ICON_FILE "app.icns"
    MACOSX_BUNDLE_GUI_IDENTIFIER "com.bmcc.homequest"
    MACOSX_BUNDLE_BUNDLE_NAME "HomeQuest"
)

set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
    MACOSX_PACKAGE_LOCATION "Resources")

qt_finalize_executable(HomeQuest)
