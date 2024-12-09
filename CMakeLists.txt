cmake_minimum_required(VERSION 3.16)
project(HomeQuest VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

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
    styles.qss
    listings.csv
    landlords.csv
    amenities.csv
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(HomeQuest
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(HomeQuest SHARED ${PROJECT_SOURCES})
    else()
        add_executable(HomeQuest ${PROJECT_SOURCES})
    endif()
endif()

target_link_libraries(HomeQuest PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)

set_target_properties(HomeQuest PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(HomeQuest)
endif()
