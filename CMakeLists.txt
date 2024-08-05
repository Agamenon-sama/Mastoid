cmake_minimum_required(VERSION 3.16)

project(Mastoid VERSION 0.2 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 COMPONENTS Quick REQUIRED)

qt_add_executable(mastoid
    src/main.cpp
    src/FileSystemHelper.h src/FileSystemHelper.cpp
    src/AppConfiguration.h src/AppConfiguration.cpp
    static/res.qrc
)

#qt_add_qml_module(mastoid
#    URI Mastoid
#    VERSION 1.0
#    QML_FILES static/ui/main.qml static/ui/FolderView.qml
#)

set_target_properties(mastoid PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_compile_definitions(mastoid
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(mastoid
    PRIVATE Qt6::Quick)
