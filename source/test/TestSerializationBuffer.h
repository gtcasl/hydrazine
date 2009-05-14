/*!
	
	\file TestSerializationBuffer.h
	
	\author Greory Diamos
	
	\date July 8, 2008
	
	\brief The header file for the TestSerializationBuffer test suite.
	
*/


#ifndef TEST_SERIALIZATION_BUFFER_H_INCLUDED
#define TEST_SERIALIZATION_BUFFER_H_INCLUDED

#ifndef ITERATIONS
#define ITERATIONS 100000
#endif

#ifndef MAX_ELEMENT_BYTES
#define MAX_ELEMENT_BYTES 100
#endif

#ifndef DEFAULT_SEED
#define DEFAULT_SEED 0
#endif

#include <common/implementation/ArgumentParser.h>
#include <common/interface/Test.h>
#include <common/implementation/SerializationBuffer.h>
#include <boost/random/mersenne_twister.hpp>

namespace test
{

	/*!
	
		\brief A test for the serialization buffer.  
		
		Write in some values
		then read them back and make sure they match.
		
		Take in an external buffer and read some values out.
		
		Write in some strings then read them out.
	
	*/
	
	class TestSerializationBuffer : public Test
	{
	
		private:
		
			typedef boost::random::mersenne_twister<uint32_t,32,351,175,19,0xccab8ee7,11,
  7,0x31b6ab00,15,0xffe50000,17, 0xa37d3c92> mersenneTwister;
		
			bool testReadWrite( std::string& stat );
			bool testExternalBuffer( std::string& stat );
			bool testStrings( std::string& stat );
		
			void performanceTest( std::string& stat );
		
			bool doTest( std::string& stat );
	
		public:
		
			TestSerializationBuffer();
			
			unsigned int iterations;
			unsigned int maxSize;
			
			unsigned int seed;
			
			bool verbose;
	
	};

}

int main( int argc, char** argv );

#endif

