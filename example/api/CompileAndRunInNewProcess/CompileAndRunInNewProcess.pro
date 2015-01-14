TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG += console
cache()

SOURCES += \
	CompileAndRunInNewProcess.cc

VUO_FRAMEWORK_PATH=../../../framework
VUO_COMPOSITION_PATH=../../composition

QMAKE_CXX = $$VUO_FRAMEWORK_PATH/Vuo.framework/MacOS/Clang/bin/clang++
QMAKE_LINK = $$QMAKE_CXX
QMAKE_CXXFLAGS += -F $$VUO_FRAMEWORK_PATH -DVUO_COMPOSITION_PATH=\\\"$$VUO_COMPOSITION_PATH\\\"
QMAKE_LFLAGS += -F $$VUO_FRAMEWORK_PATH -mmacosx-version-min=10.6
QMAKE_RPATHDIR = $$VUO_FRAMEWORK_PATH
QMAKE_LFLAGS_RPATH = -rpath$$LITERAL_WHITESPACE

# Rebuild this example when Vuo.framework's headers change.
DEPENDPATH += $$VUO_FRAMEWORK_PATH/Vuo.framework/Headers

LIBS += -framework Vuo

# In an app you plan to distribute, you should copy the Vuo framework to $${TARGET}.app/Contents/Frameworks,
# and change QMAKE_RPATHDIR to $${TARGET}.app/Contents/Frameworks.
