
# Target
TARGET      = ImageCompare

# Template
TEMPLATE    = app

# Qt Modules/Config
QT          += core gui
QT          += quickwidgets
QT          += qml quick
QT          += widgets

macx: {
# Icon
ICON        = resources/images/icons/imagecompare.icns
} else {
# Icon
ICON        = resources/images/icons/main_icon.png
}

# Soures
SOURCES     += src/main.cpp \
            src/imagecompareapp.cpp \
            src/mainwindow.cpp \
            src/compositor.cpp \
            src/compositorcontainer.cpp \
            src/sideimagecontainer.cpp \
            src/viewerwindow.cpp \
            src/aboutform.cpp \
            src/confirmationdialog.cpp \
            src/dirselectordialog.cpp \
            src/infodialog.cpp \
            src/preferencesdialog.cpp \
            src/renamefiledialog.cpp \
            src/worker.cpp \
            src/settings.cpp \
            src/utility.cpp \

# Headers
HEADERS     += src/mainwindow.h \
            src/imagecompareapp.h \
            src/compositor.h \
            src/compositorcontainer.h \
            src/sideimagecontainer.h \
            src/viewerwindow.h \
            src/aboutform.h \
            src/confirmationdialog.h \
            src/dirselectordialog.h \
            src/infodialog.h \
            src/preferencesdialog.h \
            src/renamefiledialog.h \
            src/worker.h \
            src/settings.h \
            src/defaultsettings.h \
            src/utility.h \
            src/constants.h \

# Forms
FORMS       += ui/mainwindow.ui \
            ui/viewerwindow.ui \
            ui/aboutform.ui \
            ui/confirmationdialog.ui \
            ui/dirselectordialog.ui \
            ui/infodialog.ui \
            ui/preferencesdialog.ui \
            ui/renamefiledialog.ui \


# Resources
RESOURCES   += imagecompare.qrc

# Other Files
OTHER_FILES += \
            createdmg.sh

macx: {
# Libs
LIBS        += -framework Carbon
} else {
}

# Output/Intermediate Dirs
OBJECTS_DIR = ./objs
OBJMOC      = ./objs
MOC_DIR     = ./objs
UI_DIR      = ./objs
RCC_DIR     = ./objs


