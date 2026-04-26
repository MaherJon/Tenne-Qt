cmake_minimum_required(VERSION 3.16)
project(AIDetector VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
    if(MINGW)
        add_compile_options(-O3 -DNDEBUG -flto=auto)
    endif()
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

set(ONNXRUNTIME_DIR "D:/onnxruntime")
set(ONNXRUNTIME_INCLUDE_DIR "${ONNXRUNTIME_DIR}/include")
set(ONNXRUNTIME_LIB_DIR "${ONNXRUNTIME_DIR}/lib")

if(WIN32)
    set(WIN32_EXECUTABLE TRUE)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
        set(RESOURCE_FILES resources.rc)
    endif()
endif()

find_library(ONNXRUNTIME_LIB
    NAMES onnxruntime
    PATHS ${ONNXRUNTIME_LIB_DIR}
    REQUIRED
)

set(SOURCES
    main.cpp
    mainwindow.cpp
    onnx_inference.cpp
    ${RESOURCE_FILES}
)

set(HEADERS
    mainwindow.h
    onnx_inference.h
)

set(FORMS
    mainwindow.ui
)

qt6_add_executable(AIDetector ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(AIDetector PRIVATE
    Qt6::Core
    Qt6::Widgets
    ${ONNXRUNTIME_LIB}
)

target_include_directories(AIDetector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ONNXRUNTIME_INCLUDE_DIR}
)

if(WIN32 AND EXISTS "${ONNXRUNTIME_LIB_DIR}/onnxruntime.dll")
    add_custom_command(TARGET AIDetector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ONNXRUNTIME_LIB_DIR}/onnxruntime.dll"
            $<TARGET_FILE_DIR:AIDetector>
    )
endif()