# AIDetector.pro
QT += core widgets network

CONFIG += c++17

TARGET = AIDetector
TEMPLATE = app

# 目录定义
SOURCES_DIR = src
HEADERS_DIR = include
FORMS_DIR = ui

# 构建配置
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# Windows 资源配置
win32 {
    RC_FILE = resources.rc
}

# 源文件
SOURCES += \
    $$SOURCES_DIR/main.cpp \
    $$SOURCES_DIR/mainwindow.cpp \
    $$SOURCES_DIR/onnx_inference.cpp \
    $$SOURCES_DIR/github_reporter.cpp \
    $$SOURCES_DIR/feedback_dialog.cpp

# 头文件
HEADERS += \
    $$HEADERS_DIR/mainwindow.h \
    $$HEADERS_DIR/onnx_inference.h \
    $$HEADERS_DIR/github_reporter.h \
    $$HEADERS_DIR/feedback_dialog.h

# UI 文件
FORMS += \
    $$FORMS_DIR/mainwindow.ui \
    $$FORMS_DIR/feedback_dialog.ui

# 包含路径
INCLUDEPATH += $$PWD/$$HEADERS_DIR
INCLUDEPATH += $$PWD

# ONNX Runtime 配置
win32 {
    ONNXRUNTIME_DIR = D:/onnxruntime

    exists($$ONNXRUNTIME_DIR/include/onnxruntime_c_api.h) {
        INCLUDEPATH += $$ONNXRUNTIME_DIR/include
        LIBS += -L$$ONNXRUNTIME_DIR/lib -lonnxruntime

        # 复制 DLL 到输出目录 - 修复的命令
        CONFIG(release, debug|release) {
            # Release 模式
            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$ONNXRUNTIME_DIR/lib/onnxruntime.dll) $$shell_quote($$DESTDIR) $$escape_expand(\\n\\t)
        } else {
            # Debug 模式
            QMAKE_POST_LINK += $$QMAKE_COPY $$shell_quote($$ONNXRUNTIME_DIR/lib/onnxruntime.dll) $$shell_quote($$DESTDIR) $$escape_expand(\\n\\t)
        }
    } else {
        error("ONNX Runtime not found at $$ONNXRUNTIME_DIR")
    }
}

# 编译器警告
msvc {
    QMAKE_CXXFLAGS += /W4
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra
}

# 输出目录配置
win32 {
    DESTDIR = $$PWD/bin
    MOC_DIR = $$PWD/build/moc
    RCC_DIR = $$PWD/build/rcc
    UI_HEADERS_DIR = $$PWD/build/ui
    OBJECTS_DIR = $$PWD/build/obj
}
