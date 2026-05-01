cmake_minimum_required(VERSION 3.16)
project(AIDetector)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

if(WIN32)
    set(WIN32_EXECUTABLE TRUE)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resources.rc")
        set(RESOURCE_FILES resources.rc)
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

set(ONNXRUNTIME_DIR "D:/onnxruntime")
set(ONNXRUNTIME_INCLUDE_DIR "${ONNXRUNTIME_DIR}/include")
set(ONNXRUNTIME_LIB_DIR "${ONNXRUNTIME_DIR}/lib")

if(WIN32)
    set(WIN32_EXECUTABLE TRUE)
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
    github_reporter.cpp
    feedback_dialog.cpp
    ${RESOURCE_FILES}
)

set(HEADERS
    mainwindow.h
    onnx_inference.h
    github_reporter.h
    feedback_dialog.h
)

set(FORMS
    mainwindow.ui
    feedback_dialog.ui
)

qt6_add_executable(AIDetector ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(AIDetector PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    ${ONNXRUNTIME_LIB}
)

target_include_directories(AIDetector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ONNXRUNTIME_INCLUDE_DIR}
)

if(WIN32)
    add_custom_command(TARGET AIDetector POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${ONNXRUNTIME_LIB_DIR}/onnxruntime.dll"
            $<TARGET_FILE_DIR:AIDetector>
    )
endif()