################################################################################
#	\file Makefile.am
#	\author Gregory Diamos
#	\date Saturday May 16, 2009
# 	\brief Automake input file describing the programs and sources needed to
#		Hydrazine
################################################################################

################################################################################
## Programs and Libraries
bin_PROGRAMS = 
check_PROGRAMS = TestActiveTimer TestArgumentParser TestCudaVector \
	TestMath TestPackage TestSerializableArchive \
	TestSerializationBuffer TestThread TestTimer TestXmlArgumentParser \
	TestXmlParser
lib_LIBRARIES = libhydralize.a
################################################################################

################################################################################
## libhydralize
libhydralize_a_CXXFLAGS = -I source $(CUDA_CFLAGS) -Wall -ansi -pedantic \
	-Werror -std=c++0x
libhydralize_a_SOURCES = \
	implementation/Exception.cpp \
	implementation/Package.cpp \
	implementation/SerializationBuffer.cpp \
	implementation/XmlLexer.cpp \
	implementation/Timer.cpp \
	implementation/LowLevelTimer.cpp \
	implementation/ArgumentParser.cpp \
	implementation/XmlParser.cpp \
	implementation/SerializableArchive.cpp \
	implementation/math.cpp \
	implementation/debug.cpp \
	implementation/XmlArgumentParser.cpp \
	implementation/XmlTree.cpp \
	implementation/string.cpp \
	cuda/Cuda.cpp \
	cuda/CudaException.cpp \
	interface/Configurable.cpp \
	interface/Test.cpp \
	interface/Serializable.cpp \
	interface/Stringable.cpp \
	interface/ActiveTimer.cpp \
	interface/Clonable.cpp \
	interface/Thread.cpp \
	interface/Version.cpp
################################################################################

################################################################################
## TestActiveTimer
TestActiveTimer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestActiveTimer_SOURCES = test/TestActiveTimer.cpp
TestActiveTimer_LDADD = libhydralize.a
TestActiveTimer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestArgumentParser
TestArgumentParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestArgumentParser_SOURCES = test/TestArgumentParser.cpp
TestArgumentParser_LDADD = libhydralize.a
TestArgumentParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestCudaVector
TestCudaVector_CXXFLAGS = -I source  $(CUDA_CFLAGS) -Wall -ansi -pedantic \
	-Werror -std=c++0x
TestCudaVector_SOURCES = test/TestCudaVector.cpp
TestCudaVector_LDADD = libhydralize.a
TestCudaVector_LDFLAGS = $(LIBRT) $(CUDA_LIBS)
################################################################################

################################################################################
## TestMath
TestMath_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestMath_SOURCES = test/TestMath.cpp
TestMath_LDADD = libhydralize.a
TestMath_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestPackage
TestPackage_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestPackage_SOURCES = test/TestPackage.cpp
TestPackage_LDADD = libhydralize.a
TestPackage_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestSerializableArchive
TestSerializableArchive_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestSerializableArchive_SOURCES = test/TestSerializableArchive.cpp
TestSerializableArchive_LDADD = libhydralize.a
TestSerializableArchive_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestSerializationBuffer
TestSerializationBuffer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestSerializationBuffer_SOURCES = test/TestSerializationBuffer.cpp
TestSerializationBuffer_LDADD = libhydralize.a
TestSerializationBuffer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestThread
TestThread_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestThread_SOURCES = test/TestThread.cpp
TestThread_LDADD = libhydralize.a
TestThread_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestTimer
TestTimer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestTimer_SOURCES = test/TestTimer.cpp
TestTimer_LDADD = libhydralize.a
TestTimer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestXmlArgumentParser
TestXmlArgumentParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestXmlArgumentParser_SOURCES = test/TestXmlArgumentParser.cpp
TestXmlArgumentParser_LDADD = libhydralize.a
TestXmlArgumentParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestXmlParser
TestXmlParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestXmlParser_SOURCES = test/TestXmlParser.cpp
TestXmlParser_LDADD = libhydralize.a
TestXmlParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## Tests
regression/regression.log : check
	@echo
	@echo "Running Hydrazine Regression Tests"
	@python python/RunRegression.py \
		-t regression/tests.txt \
		-l regression/regression.log -v

test : regression/regression.log
################################################################################

