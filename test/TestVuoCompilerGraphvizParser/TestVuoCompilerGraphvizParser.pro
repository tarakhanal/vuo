TEMPLATE = app
CONFIG -= app_bundle
CONFIG += console testcase graphviz VuoLLVM qtTest VuoBase VuoRuntime VuoCompiler TestVuoCompiler
TARGET = TestVuoCompilerGraphvizParser

include(../../vuo.pri)

SOURCES += \
	TestVuoCompilerGraphvizParser.cc
