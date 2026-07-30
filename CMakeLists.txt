cmake_minimum_required(VERSION 3.16)

project(Mastoid VERSION 0.3 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Qt6 6.8 COMPONENTS Quick Gui Widgets DBus Multimedia REQUIRED)
find_package(FFTW REQUIRED COMPONENTS FLOAT_LIB FLOAT_THREADS_LIB)

# set(PROJECTS_SRCS
#     src/main.cpp
#     src/FileSystemHelper.h src/FileSystemHelper.cpp
#     src/AppConfiguration.h src/AppConfiguration.cpp
#     src/SystemTrayMenu.h src/SystemTrayMenu.cpp
#     src/MprisIntegration.h src/MprisIntegration.cpp
# )


qt_add_executable(mastoid
    src/main.cpp
    src/FileSystemHelper.h src/FileSystemHelper.cpp
    src/AppConfiguration.h src/AppConfiguration.cpp
    src/SystemTrayMenu.h src/SystemTrayMenu.cpp
    src/MprisIntegration.h src/MprisIntegration.cpp
    src/SpectrumAnalyzer.h src/SpectrumAnalyzer.cpp
    src/RingBuffer.h src/RingBuffer.cpp
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

target_include_directories(mastoid PRIVATE ${FFTW_INCLUDE_DIRS})

target_compile_definitions(mastoid
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(mastoid
    PRIVATE Qt6::Quick Qt6::Gui Qt6::Widgets Qt6::DBus Qt6::Multimedia
)
target_link_libraries(mastoid PRIVATE ${FFTW_LIBRARIES})
