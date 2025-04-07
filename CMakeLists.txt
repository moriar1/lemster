cmake_minimum_required(VERSION 3.16)

project(lemster VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Sql)
find_package(Qt6 REQUIRED COMPONENTS Network)
qt_standard_project_setup()

qt_add_executable(lemster
  src/main.cpp
  src/centralwidget.h
  src/centralwidget.cpp
  src/addpostdialog.h
  src/addpostdialog.cpp
  src/mainwindow.h
  src/mainwindow.cpp
  src/logindialog.h
  src/logindialog.cpp
)

target_link_libraries(lemster PRIVATE Qt6::Widgets)
target_link_libraries(lemster PRIVATE Qt6::Sql)
target_link_libraries(lemster PRIVATE Qt6::Network)

target_include_directories(lemster PRIVATE include)
target_compile_options(lemster PUBLIC -Wall -Wextra -pedantic)

set_target_properties(lemster PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

install(TARGETS lemster DESTINATION bin)
