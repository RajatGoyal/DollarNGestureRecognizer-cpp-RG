
TEMPLATE = app
INCLUDEPATH += ./lib

# Input
HEADERS +=  dollar/GestureTemplate.h \
    dollar/PathWriter.h \
    dollar/GeometricRecognizerTypes.h \
    dollar/GeometricRecognizer.h \
    dollar/SampleGestures.h \
    lib/GestureTemplate.h \
    lib/PathWriter.h \
    lib/GeometricRecognizerTypes.h \
    lib/GeometricRecognizer.h \
    lib/SampleGestures.h \
    lib/MultiStrokeGestureTemplate .h \
    lib/MultiStrokeGestureTemplate .h \
    lib/MultiStrokeGestureTemplate .h \
    lib/MultiStrokeGestureTemplate .h \
    lib/SampleMultiStrokeGestures.h \
    lib/MultipleStrokeGestureTemplate.h \
    lib/utils.h
SOURCES += main.cpp \
    lib/GeometricRecognizer.cpp

LIBS += -L/usr/lib \
    -lml \
