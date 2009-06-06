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
	TestXmlParser TestBTree
lib_LIBRARIES = libhydralize.a
################################################################################

################################################################################
## libhydralize
libhydralize_a_CXXFLAGS = -I source $(CUDA_CFLAGS) -Wall -ansi -pedantic \
	-Werror -std=c++0x
libhydralize_a_SOURCES = \
	hydrazine/implementation/Exception.cpp \
	hydrazine/implementation/Package.cpp \
	hydrazine/implementation/SerializationBuffer.cpp \
	hydrazine/implementation/XmlLexer.cpp \
	hydrazine/implementation/Timer.cpp \
	hydrazine/implementation/LowLevelTimer.cpp \
	hydrazine/implementation/ArgumentParser.cpp \
	hydrazine/implementation/XmlParser.cpp \
	hydrazine/implementation/SerializableArchive.cpp \
	hydrazine/implementation/math.cpp \
	hydrazine/implementation/debug.cpp \
	hydrazine/implementation/XmlArgumentParser.cpp \
	hydrazine/implementation/XmlTree.cpp \
	hydrazine/implementation/string.cpp \
	hydrazine/cuda/Cuda.cpp \
	hydrazine/cuda/CudaException.cpp \
	hydrazine/interface/Configurable.cpp \
	hydrazine/interface/Test.cpp \
	hydrazine/interface/Serializable.cpp \
	hydrazine/interface/Stringable.cpp \
	hydrazine/interface/ActiveTimer.cpp \
	hydrazine/interface/Clonable.cpp \
	hydrazine/interface/Thread.cpp \
	hydrazine/interface/Version.cpp
################################################################################

################################################################################
## TestActiveTimer
TestActiveTimer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestActiveTimer_SOURCES = hydrazine/test/TestActiveTimer.cpp
TestActiveTimer_LDADD = libhydralize.a
TestActiveTimer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestArgumentParser
TestArgumentParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestArgumentParser_SOURCES = hydrazine/test/TestArgumentParser.cpp
TestArgumentParser_LDADD = libhydralize.a
TestArgumentParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestBTree
TestBTree_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestBTree_SOURCES = hydrazine/test/TestBTree.cpp
TestBTree_LDADD = libhydralize.a
TestBTree_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestCudaVector
TestCudaVector_CXXFLAGS = -I source  $(CUDA_CFLAGS) -Wall -ansi -pedantic \
	-Werror -std=c++0x
TestCudaVector_SOURCES = hydrazine/test/TestCudaVector.cpp
TestCudaVector_LDADD = libhydralize.a
TestCudaVector_LDFLAGS = $(LIBRT) $(CUDA_LIBS)
################################################################################

################################################################################
## TestMath
TestMath_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestMath_SOURCES = hydrazine/test/TestMath.cpp
TestMath_LDADD = libhydralize.a
TestMath_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestPackage
TestPackage_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestPackage_SOURCES = hydrazine/test/TestPackage.cpp
TestPackage_LDADD = libhydralize.a
TestPackage_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestSerializableArchive
TestSerializableArchive_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestSerializableArchive_SOURCES = hydrazine/test/TestSerializableArchive.cpp
TestSerializableArchive_LDADD = libhydralize.a
TestSerializableArchive_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestSerializationBuffer
TestSerializationBuffer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestSerializationBuffer_SOURCES = hydrazine/test/TestSerializationBuffer.cpp
TestSerializationBuffer_LDADD = libhydralize.a
TestSerializationBuffer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestThread
TestThread_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestThread_SOURCES = hydrazine/test/TestThread.cpp
TestThread_LDADD = libhydralize.a
TestThread_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestTimer
TestTimer_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestTimer_SOURCES = hydrazine/test/TestTimer.cpp
TestTimer_LDADD = libhydralize.a
TestTimer_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestXmlArgumentParser
TestXmlArgumentParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror \
	-std=c++0x
TestXmlArgumentParser_SOURCES = hydrazine/test/TestXmlArgumentParser.cpp
TestXmlArgumentParser_LDADD = libhydralize.a
TestXmlArgumentParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## TestXmlParser
TestXmlParser_CXXFLAGS = -I source -Wall -ansi -pedantic -Werror -std=c++0x
TestXmlParser_SOURCES = hydrazine/test/TestXmlParser.cpp
TestXmlParser_LDADD = libhydralize.a
TestXmlParser_LDFLAGS = $(LIBRT)
################################################################################

################################################################################
## Tests
regression/regression.log : check
	@echo
	@echo "Running Hydrazine Regression Tests"
	@python hydrazine/python/RunRegression.py \
		-t regression/tests.txt \
		-l regression/regression.log -v

test : regression/regression.log
################################################################################

