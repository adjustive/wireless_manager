QT += core gui

TARGET = wireless_manager
VERSION = 0.0.1

QMAKE_CFLAGS += -EL $(RMCFLAGS)
QMAKE_CXXFLAGS += -EL $(RMCFLAGS)
QMAKE_LFLAGS += -EL

#INCLUDEPATH += $(LAUNCH_DIR)/include

TEMPLATE = app
SOURCES += main.cpp \
        mainwindow.cpp \
        wireless.cpp  \
        key_filter.cpp  \
        password_qdialog.cpp

HEADERS  += mainwindow.h \
        wireless.h \
        key_filter.h \
        password_qdialog.h

Debug:DEFINES += DEBUG

