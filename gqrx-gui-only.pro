TEMPLATE = app
TARGET = gui
INCLUDEPATH += .
INCPATH += src/
PREFIX = /usr/local


QT  += core gui network widgets svg

lessThan(QT_MAJOR_VERSION,5) {
    error("Gqrx requires Qt 5.")
}



QMAKE_CFLAGS += -fdiagnostics-color=auto
QMAKE_CXXFLAGS += -fdiagnostics-color=auto

#PKGCONFIG_EXISTS = $$system(pkg-config --version)
#isEmpty(PKGCONFIG_EXISTS) {
#    error("Gqrx requires pkg-config to build.")
#}

    TARGET = gui

# enable pkg-config to find dependencies
#CONFIG += link_pkgconfig

# May have to include Pulseaudio stuff?

RESOURCES += \
    resources/icons.qrc 
 #   resources/textfiles.qrc

# make clean target
QMAKE_CLEAN += gui

# make install target
isEmpty(PREFIX) {
    message("No prefix given. Using /usr/local")
    PREFIX=/usr/local
}

target.path  = $$PREFIX/bin
INSTALLS    += target

#CONFIG += debug

# disable debug messages in release
CONFIG(debug, debug|release) {
    # Use for valgrind
    #QMAKE_CFLAGS_DEBUG += '-g -O0'

    # Define version string (see below for releases)
    VER = $$system(git describe --abbrev=8)
   
} else {
    DEFINES += QT_NO_DEBUG
    DEFINES += QT_NO_DEBUG_OUTPUT
    VER = $$system(git describe --abbrev=1)
  
}


# Input

HEADERS +=	freqctrl.h \
			plotter.h \
	#		qtcolorpicker.h \
			gqrx.h \
			mainwindow.h \
			meter.h 

FORMS += mainwindow.ui 
         
SOURCES +=  freqctrl.cpp \
			meter.cpp \
			plotter.cpp \
	#		qtcolorpicker.cpp \
			main.cpp \
			mainwindow.cpp 
           





